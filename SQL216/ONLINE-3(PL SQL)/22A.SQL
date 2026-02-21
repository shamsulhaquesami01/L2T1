CREATE OR REPLACE PROCEDURE LONGEST_SERVING_EMPLOYEE(RG IN VARCHAR2) IS
JDATE DATE;
NAAM VARCHAR2(100);
TITEL VARCHAR2(100);
CANTRY VARCHAR2(100); DEPT VARCHAR2(100); SITY VARCHAR2(100);
YEARS NUMBER;
MIN_YEAR NUMBER;
BEGIN
    MIN_YEAR:=0;
FOR R IN ( SELECT HIRE_DATE,FIRST_NAME||LAST_NAME AS FULLNAME,
COUNTRY_NAME,DEPARTMENT_NAME, CITY,JOB_title
FROM EMPLOYEES JOIN DEPARTMENTS ON DEPARTMENTS.DEPARTMENT_ID=EMPLOYEES.DEPARTMENT_ID 
JOIN LOCATIONS ON DEPARTMENTS.LOCATION_ID = LOCATIONS.LOCATION_ID
JOIN COUNTRIES ON LOCATIONS.COUNTRY_ID=COUNTRIES.COUNTRY_ID
JOIN REGIONS ON COUNTRIES.REGION_ID=REGIONS.REGION_ID
JOIN JOBS ON JOBS.JOB_ID = EMPLOYEES.JOB_ID
WHERE RG= REGION_NAME )
LOOP
YEARS := (MONTHS_BETWEEN(SYSDATE, R.HIRE_DATE) / 12) ;
IF YEARS > MIN_YEAR THEN
MIN_YEAR := YEARS;
naam := R.FULLNAME;
TITEL:=R.JOB_title;
CANTRY := R.COUNTRY_NAME;
DEPT := R.DEPARTMENT_NAME;
SITY := R.CITY;
END IF ;
END LOOP ;
DBMS_OUTPUT.PUT_LINE(RG ||ROUND( MIN_YEAR)||NAAM||TITEL||CANTRY||DEPT||SITY);
END;
/    

EXECUTE LONGEST_SERVING_EMPLOYEE('Americas');kjhgf
SELECT * FROM EMPLOYEES;
SELECT * FROM REGIONS;
SELECT * FROM DEPARTMENTS;
SELECT * FROM LOCATIONS;
SELECT * FROM JOBS;




CREATE OR REPLACE PROCEDURE RANK_JOBS IS
BEGIN
    -- Using a cursor loop handles the 'INTO' and 'Multiple Rows' issues automatically
    FOR R IN (
        SELECT job_title, emp_count, min_salary, max_salary, avg_salary,
               RANK() OVER (ORDER BY emp_count DESC, avg_salary DESC) as rnk
        FROM (
            -- Subquery to compute counts/avgs so the aliases are available for RANK()
            SELECT J.Job_title, 
                   (SELECT COUNT(*) FROM employees e2 WHERE e2.JOB_ID = j.JOB_ID) as emp_count, 
                   j.min_salary, 
                   j.max_salary, 
                   (SELECT AVG(e2.salary) FROM employees e2 WHERE e2.JOB_ID = j.JOB_ID) as avg_salary
            FROM JOBS J
        )
    )
    LOOP
        -- Access fields via the record variable 'R'
        DBMS_OUTPUT.PUT_LINE('rank= ' || R.rnk || ' | job = ' || R.job_title || ' | emp count = ' || R.emp_count);
    END LOOP;
END;
/

EXECUTE RANK_JOBS;


CREATE OR REPLACE TRIGGER TRG_EMPLOYEE_LEAVE
AFTER DELETE ON EMPLOYEES
FOR EACH ROW
DECLARE
    v_substitute_id NUMBER := NULL;
    v_is_manager    NUMBER := 0;
    v_my_sub_count  NUMBER := 0;
BEGIN
    -- Check if the deleted employee was a manager
    SELECT COUNT(*) INTO v_is_manager 
    FROM EMPLOYEES 
    WHERE MANAGER_ID = :OLD.EMPLOYEE_ID;

    IF v_is_manager > 0 THEN
        -- Condition 2 & 3: Employee is a manager (takes priority)
        -- Get the subordinate count of the deleted manager
        v_my_sub_count := v_is_manager;

        BEGIN
            SELECT emp_id INTO v_substitute_id
            FROM (
                SELECT E.EMPLOYEE_ID as emp_id, COUNT(S.EMPLOYEE_ID) as sub_count
                FROM EMPLOYEES E
                LEFT JOIN EMPLOYEES S ON E.EMPLOYEE_ID = S.MANAGER_ID
                WHERE E.EMPLOYEE_ID <> :OLD.EMPLOYEE_ID
                GROUP BY E.EMPLOYEE_ID
                ORDER BY ABS(COUNT(S.EMPLOYEE_ID) - v_my_sub_count) ASC, E.EMPLOYEE_ID ASC
            )
            WHERE ROWNUM = 1;
        EXCEPTION WHEN NO_DATA_FOUND THEN v_substitute_id := NULL;
        END;

    ELSIF :OLD.MANAGER_ID IS NOT NULL THEN
        -- Condition 1: Has a manager but is not a manager themselves
        BEGIN
            SELECT EMPLOYEE_ID INTO v_substitute_id
            FROM (
                SELECT EMPLOYEE_ID
                FROM EMPLOYEES
                WHERE MANAGER_ID = :OLD.MANAGER_ID
                AND EMPLOYEE_ID <> :OLD.EMPLOYEE_ID
                ORDER BY ABS(SALARY - :OLD.SALARY) ASC, EMPLOYEE_ID ASC
            )
            WHERE ROWNUM = 1;
        EXCEPTION WHEN NO_DATA_FOUND THEN v_substitute_id := NULL;
        END;
    END IF;

    -- Final Insert into Leaves table
    INSERT INTO Leaves (
        employee_id, 
        employee_working_instead_of_him_her, 
        current_date
    ) VALUES (
        :OLD.EMPLOYEE_ID, 
        v_substitute_id, 
        SYSDATE
    );
END;
/