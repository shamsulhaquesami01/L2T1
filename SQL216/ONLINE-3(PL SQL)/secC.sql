
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

CREATE OR REPLACE FUNCTION proc_name(eid IN NUMBER) return number is
    exists number;
    max_sal number;
    jid number;
BEGIN
    select count(*) from employees where employee_id=eid;
    select salary into sal from
    select job_id into jid from employees where employee_id=eid;
    select max_sal from jobs where JOB_ID = jid;
    if(count=0) then return null;
    if (GET_YEARS(eid) >=10) then 
    if()

    END LOOP;
    COMMIT;
EXCEPTION
    WHEN NO_DATA_FOUND THEN DBMS_OUTPUT.PUT_LINE('Not found.');
    WHEN OTHERS THEN DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/