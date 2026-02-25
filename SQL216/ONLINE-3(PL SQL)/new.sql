
CREATE OR REPLACE TRIGGER trg_name
BEFORE UPDATE OF salary ON employees
FOR EACH ROW
DECLARE
    dpet number;
    tot_sal number;
BEGIN
    SELECT DEPARTMENT_ID INTO dpet FROM EMPLOYEES WHERE DEPARTMENT_ID=:old.department_id;
      -- validate on original table
    SELECT SUM(salary) into tot_sal from employees where department_id=dpet and employee_id <> :old.employee_id;

    IF (tot_sal+:new.salary) > 50000 THEN
        RAISE_APPLICATION_ERROR(-20001, 'message');
    END IF;
EXCEPTION
    WHEN NO_DATA_FOUND THEN DBMS_OUTPUT.PUT_LINE('Not found.');
    WHEN OTHERS THEN RAISE;
END;
/