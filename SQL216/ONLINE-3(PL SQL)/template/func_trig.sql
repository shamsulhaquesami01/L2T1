CREATE OR REPLACE PROCEDURE proc_name(param IN NUMBER) IS
    -- variables
BEGIN
    FOR R IN (SELECT ... FROM ... WHERE ... ORDER BY ...) LOOP
        -- logic
        -- UPDATE or INSERT
    END LOOP;
    COMMIT;
EXCEPTION
    WHEN NO_DATA_FOUND THEN DBMS_OUTPUT.PUT_LINE('Not found.');
    WHEN OTHERS THEN DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/


CREATE OR REPLACE TRIGGER trg_name
BEFORE UPDATE OF col ON table_name
FOR EACH ROW
DECLARE
    v_var NUMBER;
BEGIN
    SELECT ... INTO v_var FROM EMPLOYEES WHERE ...;  -- validate on original table
    IF condition THEN
        RAISE_APPLICATION_ERROR(-20001, 'message');
    END IF;
    :NEW.col := corrected_value;  -- auto-correct if needed
EXCEPTION
    WHEN NO_DATA_FOUND THEN DBMS_OUTPUT.PUT_LINE('Not found.');
    WHEN OTHERS THEN RAISE;
END;
/

------------------------------------------------
-- Named exceptions
WHEN NO_DATA_FOUND THEN      -- SELECT INTO returned nothing
WHEN TOO_MANY_ROWS THEN      -- SELECT INTO returned more than 1 row
WHEN ZERO_DIVIDE THEN        -- division by zero
WHEN OTHERS THEN             -- catches everything else

-- Custom exception
RAISE_APPLICATION_ERROR(-20001, 'message');  -- -20000 to -20999 only

-- Re-raise same exception
WHEN OTHERS THEN RAISE;

-- Get error message
SQLERRM    -- current error message string
SQLCODE    -- current error code number



----------------------------------------------------------------------
-- Lowest paid under a manager
SELECT employee_id FROM (
    SELECT employee_id FROM EMPLOYEES
    WHERE manager_id = x
    ORDER BY salary ASC
) WHERE ROWNUM = 1;

-- Closest salary to a target
SELECT employee_id FROM (
    SELECT employee_id FROM EMPLOYEES
    WHERE manager_id = x
    AND employee_id <> target_id
    ORDER BY ABS(salary - target_sal) ASC
) WHERE ROWNUM = 1;

-- Option A: counter in FOR loop (simplest)
rank_num := rank_num + 1;

-- Option B: RANK() OVER
RANK() OVER (ORDER BY col1 DESC, col2 DESC) AS RNK

-- Option C: ROWNUM on subquery
SELECT ROWNUM AS RNK, city, avg_sal FROM (
    SELECT city, avg_sal FROM ... ORDER BY avg_sal DESC
);


-- Is this employee a manager (has subordinates)?
SELECT COUNT(*) INTO s_count FROM EMPLOYEES WHERE MANAGER_ID = eid;

-- Is this employee official dept head?
SELECT COUNT(*) INTO is_mgr FROM DEPARTMENTS 
WHERE DEPARTMENT_ID = dept_id AND MANAGER_ID = eid;


MONTHS_BETWEEN(SYSDATE, HIRE_DATE)        -- total months
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) / 12   -- total years
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 12   -- past probation
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 60   -- 5 years


-- BEFORE trigger → can modify :NEW values
:NEW.SALARY := corrected_value;  -- works in BEFORE only

-- Block an operation
RAISE_APPLICATION_ERROR(-20001, 'message');

-- Avoid mutating table
-- Fire trigger on EMPLOYEES_COPY, validate against EMPLOYEES

-- Autonomous transaction (if you must UPDATE same table)
DECLARE
    PRAGMA AUTONOMOUS_TRANSACTION;
BEGIN
    UPDATE ...;
    COMMIT;
END;

-- :OLD and :NEW only, never :variable
:OLD.salary  :NEW.department_id  -- correct
:my_var      -- WRONG