CREATE OR REPLACE TRIGGER TRG_DEPT_SALARY_CAP
BEFORE UPDATE OF SALARY ON EMPLOYEES
FOR EACH ROW
DECLARE
    tot_sal NUMBER;
BEGIN
    
    SELECT SUM(SALARY) INTO tot_sal
    FROM EMPLOYEES
    WHERE DEPARTMENT_ID = :OLD.DEPARTMENT_ID
    AND EMPLOYEE_ID <> :OLD.EMPLOYEE_ID;

    
    IF (NVL(tot_sal, 0) + :NEW.SALARY) > 50000 THEN
        RAISE_APPLICATION_ERROR(-20001, 
            'Department salary cap of $50,000 exceeded!');
    END IF;

EXCEPTION
    WHEN NO_DATA_FOUND THEN 
        DBMS_OUTPUT.PUT_LINE('Department not found.');
    WHEN OTHERS THEN RAISE;
END;
/


CREATE OR REPLACE FUNCTION GET_YEARS_FLOOR(eid IN NUMBER)
RETURN NUMBER IS
    yrs NUMBER;
BEGIN
    SELECT FLOOR(MONTHS_BETWEEN(SYSDATE, HIRE_DATE) / 12)
    INTO yrs FROM EMPLOYEES WHERE EMPLOYEE_ID = eid;
    RETURN yrs;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/



CREATE OR REPLACE FUNCTION IS_OVERPAID(eid IN NUMBER)
RETURN NUMBER IS
    emp_count  NUMBER;
    yrs        NUMBER;
    emp_sal    NUMBER;
    max_sal    NUMBER;
    allowed    NUMBER;
    emp_job    VARCHAR2(20);
BEGIN
    
    SELECT COUNT(*) INTO emp_count
    FROM EMPLOYEES WHERE EMPLOYEE_ID = eid;

    IF emp_count = 0 THEN
        RETURN NULL;          
    END IF;

    
    SELECT SALARY, JOB_ID
    INTO emp_sal, emp_job
    FROM EMPLOYEES WHERE EMPLOYEE_ID = eid;

    
    BEGIN
        SELECT MAX_SALARY INTO max_sal
        FROM JOBS WHERE JOB_ID = emp_job;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RETURN -1;        
    END;

   
    IF max_sal IS NULL THEN
        RETURN -1;
    END IF;

    
    yrs := GET_YEARS_FLOOR(eid);

   
    IF yrs >= 10 THEN
        allowed := max_sal * 0.95;
    ELSIF yrs >= 5 THEN
        allowed := max_sal * 0.90;
    ELSE
        allowed := max_sal * 0.85;
    END IF;

    -- Return result
    IF emp_sal > allowed THEN
        RETURN 1;             
    ELSE
        RETURN 0;            
    END IF;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
        RETURN NULL;
END;
/
