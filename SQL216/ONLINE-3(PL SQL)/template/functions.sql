-- =============================================
-- FUNCTION 1: SUBORDINATE COUNT
-- =============================================
CREATE OR REPLACE FUNCTION GET_SUB_COUNT(eid IN NUMBER) 
RETURN NUMBER IS
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO cnt FROM EMPLOYEES WHERE MANAGER_ID = eid;
    RETURN cnt;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF GET_SUB_COUNT(:OLD.EMPLOYEE_ID) > 0 THEN  -- is a manager
IF GET_SUB_COUNT(mgr_id) >= 5 THEN           -- manages 5+
s_count := GET_SUB_COUNT(R.EMPLOYEE_ID);     -- store count

---subordite in department
CREATE OR REPLACE FUNCTION COUNT_SUBS_IN_DEPT(mid IN NUMBER, did IN NUMBER) 
RETURN NUMBER IS
    v_cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_cnt FROM EMPLOYEES 
    WHERE MANAGER_ID = mid AND DEPARTMENT_ID = did;
    RETURN v_cnt;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/


-- =============================================
-- FUNCTION 2: YEARS WORKED
-- =============================================
CREATE OR REPLACE FUNCTION GET_YEARS(eid IN NUMBER) 
RETURN NUMBER IS
    yrs NUMBER;
BEGIN
    SELECT MONTHS_BETWEEN(SYSDATE, HIRE_DATE) / 12 
    INTO yrs FROM EMPLOYEES WHERE EMPLOYEE_ID = eid;
    RETURN yrs;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF GET_YEARS(eid) >= 5 THEN      -- worked 5+ years
IF GET_YEARS(eid) <= 1 THEN      -- probation period
bonus := TRUNC(GET_YEARS(eid)/n)*m;  -- bonus calc


-- =============================================
-- FUNCTION 3: IS PERMANENT (past 1 year probation)
-- =============================================
CREATE OR REPLACE FUNCTION IS_PERMANENT(eid IN NUMBER) 
RETURN NUMBER IS
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO cnt FROM EMPLOYEES 
    WHERE EMPLOYEE_ID = eid 
    AND MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 12;
    RETURN cnt;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF IS_PERMANENT(R.EMPLOYEE_ID) = 1 THEN   -- is permanent
IF IS_PERMANENT(eid) = 0 THEN CONTINUE;   -- skip non-permanent


-- =============================================
-- FUNCTION 4: IS MANAGER (official dept head)
-- =============================================
CREATE OR REPLACE FUNCTION IS_DEPT_MANAGER(eid IN NUMBER, did IN NUMBER) 
RETURN NUMBER IS
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO cnt FROM DEPARTMENTS 
    WHERE DEPARTMENT_ID = did AND MANAGER_ID = eid;
    RETURN cnt;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF IS_DEPT_MANAGER(:OLD.EMPLOYEE_ID, :OLD.DEPARTMENT_ID) = 1 THEN


-- =============================================
-- FUNCTION 5: MIN SALARY IN DEPT
-- =============================================
CREATE OR REPLACE FUNCTION GET_DEPT_MIN_SAL(did IN NUMBER) 
RETURN NUMBER IS
    min_sal NUMBER;
BEGIN
    SELECT MIN(SALARY) INTO min_sal FROM EMPLOYEES 
    WHERE DEPARTMENT_ID = did;
    RETURN NVL(min_sal, 0);
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF :OLD.SALARY < GET_DEPT_MIN_SAL(:NEW.DEPARTMENT_ID) THEN


-- =============================================
-- FUNCTION 6: MAX SALARY IN DEPT
-- =============================================
CREATE OR REPLACE FUNCTION GET_DEPT_MAX_SAL(did IN NUMBER) 
RETURN NUMBER IS
    max_sal NUMBER;
BEGIN
    SELECT MAX(SALARY) INTO max_sal FROM EMPLOYEES 
    WHERE DEPARTMENT_ID = did;
    RETURN NVL(max_sal, 0);
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF new_sal > GET_DEPT_MAX_SAL(R.DEPARTMENT_ID) THEN
    new_sal := GET_DEPT_MAX_SAL(R.DEPARTMENT_ID);  -- cap salary
END IF;


-- =============================================
-- FUNCTION 7: LOWEST SALARY EMPLOYEE ID IN DEPT
-- =============================================
CREATE OR REPLACE FUNCTION GET_LOWEST_EMP(did IN NUMBER) 
RETURN NUMBER IS
    eid NUMBER;
BEGIN
    SELECT EMPLOYEE_ID INTO eid FROM (
        SELECT EMPLOYEE_ID FROM EMPLOYEES
        WHERE DEPARTMENT_ID = did
        ORDER BY SALARY ASC, HIRE_DATE ASC  -- tie-break by earliest hire
    ) WHERE ROWNUM = 1;
    RETURN eid;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
lowest_id := GET_LOWEST_EMP(R.DEPARTMENT_ID);


-- =============================================
-- FUNCTION 8: LOWEST SALARY EMP ID UNDER MANAGER
-- =============================================
CREATE OR REPLACE FUNCTION GET_LOWEST_UNDER_MGR(mid IN NUMBER) 
RETURN NUMBER IS
    eid NUMBER;
BEGIN
    SELECT EMPLOYEE_ID INTO eid FROM (
        SELECT EMPLOYEE_ID FROM EMPLOYEES
        WHERE MANAGER_ID = mid
        ORDER BY SALARY ASC
    ) WHERE ROWNUM = 1;
    RETURN eid;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
eid1 := GET_LOWEST_UNDER_MGR(m1);
eid2 := GET_LOWEST_UNDER_MGR(m2);


-- =============================================
-- FUNCTION 9: CLOSEST SALARY EMP UNDER SAME MANAGER
-- =============================================
CREATE OR REPLACE FUNCTION GET_CLOSEST_SAL_EMP(eid IN NUMBER, mid IN NUMBER, sal IN NUMBER)
RETURN NUMBER IS
    closest_id NUMBER;
BEGIN
    SELECT EMPLOYEE_ID INTO closest_id FROM (
        SELECT EMPLOYEE_ID FROM EMPLOYEES
        WHERE MANAGER_ID = mid
        AND EMPLOYEE_ID <> eid          -- exclude self
        ORDER BY ABS(SALARY - sal) ASC  -- closest salary
    ) WHERE ROWNUM = 1;
    RETURN closest_id;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
-- When employee leaves, find replacement
replacement := GET_CLOSEST_SAL_EMP(:OLD.EMPLOYEE_ID, 
                                    :OLD.MANAGER_ID, 
                                    :OLD.SALARY);


-- =============================================
-- FUNCTION 10: MANAGER ID OF DEPT
-- =============================================
CREATE OR REPLACE FUNCTION GET_DEPT_MANAGER(did IN NUMBER) 
RETURN NUMBER IS
    mid NUMBER;
BEGIN
    SELECT MANAGER_ID INTO mid FROM DEPARTMENTS 
    WHERE DEPARTMENT_ID = did;
    RETURN mid;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
mgr := GET_DEPT_MANAGER(:NEW.DEPARTMENT_ID);
IF :OLD.EMPLOYEE_ID = GET_DEPT_MANAGER(:OLD.DEPARTMENT_ID) THEN


-- =============================================
-- FUNCTION 11: JOB SALARY MIDPOINT
-- =============================================
CREATE OR REPLACE FUNCTION GET_JOB_MIDPOINT(jid IN VARCHAR2) 
RETURN NUMBER IS
    mid NUMBER;
BEGIN
    SELECT (MIN_SALARY + MAX_SALARY) / 2 INTO mid 
    FROM JOBS WHERE JOB_ID = jid;
    RETURN mid;
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/

-- USAGE:
IF emp_salary > GET_JOB_MIDPOINT(emp_job_id) THEN  -- above midpoint


-- =============================================
-- FUNCTION 12: JOB COUNT (including current)
-- =============================================
CREATE OR REPLACE FUNCTION GET_JOB_COUNT(eid IN NUMBER) 
RETURN NUMBER IS
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) + 1 INTO cnt   -- +1 for current job
    FROM JOB_HISTORY WHERE EMPLOYEE_ID = eid;
    RETURN cnt;
EXCEPTION WHEN OTHERS THEN RETURN 1;
END;
/

-- USAGE:
IF GET_JOB_COUNT(R.EMPLOYEE_ID) >= MIN_JOB_COUNT THEN


-- =============================================
-- FUNCTION 13: MANAGER CRED STRING
-- =============================================
CREATE OR REPLACE FUNCTION GET_MGR_CRED(email IN VARCHAR2, emp_count IN NUMBER)
RETURN VARCHAR2 IS
BEGIN
    RETURN SUBSTR(email,1,2) || '**' || SUBSTR(email,-2,2) || '-' || emp_count;
END;
/

-- USAGE:
cred := GET_MGR_CRED(R.EMAIL, GET_SUB_COUNT(R.EMPLOYEE_ID));
UPDATE TEMP_EMPLOYEES SET MANAGER_CRED = GET_MGR_CRED(R.EMAIL, s_count)
WHERE EMPLOYEE_ID = R.EMPLOYEE_ID;


-- =============================================
-- FUNCTION 14: HIGHEST PAID EMP ID UNDER MANAGER
-- =============================================
CREATE OR REPLACE FUNCTION GET_HIGHEST_UNDER_MGR(mid IN NUMBER)
RETURN NUMBER IS
    eid NUMBER;
BEGIN
    SELECT EMPLOYEE_ID INTO eid FROM (
        SELECT EMPLOYEE_ID FROM EMPLOYEES
        WHERE MANAGER_ID = mid
        ORDER BY SALARY DESC
    ) WHERE ROWNUM = 1;
    RETURN eid;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
top_emp := GET_HIGHEST_UNDER_MGR(:OLD.EMPLOYEE_ID);


-- =============================================
-- FUNCTION 15: MANAGER WITH FEWEST SUBORDINATES IN DEPT
-- =============================================
CREATE OR REPLACE FUNCTION GET_MGR_FEWEST_SUBS(did IN NUMBER)
RETURN NUMBER IS
    mid NUMBER;
BEGIN
    SELECT MANAGER_ID INTO mid FROM (
        SELECT MANAGER_ID, COUNT(*) AS cnt
        FROM EMPLOYEES
        WHERE DEPARTMENT_ID = did
        AND MANAGER_ID IS NOT NULL
        GROUP BY MANAGER_ID
        ORDER BY cnt ASC
    ) WHERE ROWNUM = 1;
    RETURN mid;
EXCEPTION WHEN OTHERS THEN RETURN NULL;
END;
/

-- USAGE:
:NEW.MANAGER_ID := GET_MGR_FEWEST_SUBS(:NEW.DEPARTMENT_ID);

--=========
--city of dept

CREATE OR REPLACE FUNCTION GET_DEPT_CITY(did IN NUMBER) 
RETURN VARCHAR2 IS
    v_city VARCHAR2(100);
BEGIN
    SELECT l.CITY INTO v_city FROM DEPARTMENTS d
    JOIN LOCATIONS l ON d.LOCATION_ID = l.LOCATION_ID
    WHERE d.DEPARTMENT_ID = did;
    RETURN v_city;
EXCEPTION WHEN OTHERS THEN RETURN 'Unknown';
END;
/

-- 21. Min Salary of Managers in a Specific Job


SQL
CREATE OR REPLACE FUNCTION GET_MIN_MGR_SAL_FOR_JOB(jid IN VARCHAR2)
RETURN NUMBER IS
    v_min NUMBER;
BEGIN
    SELECT MIN(SALARY) INTO v_min FROM EMPLOYEES
    WHERE JOB_ID = jid AND EMPLOYEE_ID IN (SELECT DISTINCT MANAGER_ID FROM EMPLOYEES);
    RETURN NVL(v_min, 0);
EXCEPTION WHEN OTHERS THEN RETURN 0;
END;
/