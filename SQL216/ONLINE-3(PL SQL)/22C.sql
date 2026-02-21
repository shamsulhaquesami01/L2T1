create or replace function is_ready_for_promotion(eid in number) 
return  varchar2 is
id number;
BEGIN
    for R in (select e.employee_id  from employees e 
    where MONTHS_BETWEEN(sysdate,e.HIRE_DATE) >=60 AND
    e.salary > (select (j.min_salary+j.max_salary)/2 from jobs j where e.job_id=j.job_id) and
    1<=(select count(e2.employee_id) from employees e2 where e2.MANAGER_ID=e.employee_id))
    loop
 IF eid = R.employee_id THEN 
            RETURN 'yes';
        END IF;
    END LOOP;
    RETURN 'no';
EXCEPTION
WHEN NO_DATA_FOUND THEN
RETURN 'No employee found.' ;
when others THEN
return 'i donnnn know';
end;
/


DECLARE
msg varchar2(100);
BEGIN
    msg :=IS_READY_FOR_PROMOTION(100);
    DBMS_OUTPUT.PUT_LINE('Function status: ' || msg);
END;
/

SELECT EMPLOYEE_ID,COMMISSION_PCT from EMPLOYEES_COPY;


create or replace PROCEDURE update_salary IS
newsewt date;
BEGIN
    select max(hire_date) into newsewt from EMPLOYEES;
    update EMPLOYEES_COPY e
    set e.salary =
    CASE
   when ( e.SALARY + (NVL(e.COMMISSION_PCT,0) * e.SALARY) + 0.1 * (select j.min_salary from jobs j
    where j.job_id=e.JOB_ID) + 0.1 * (select avg(e2.salary) from EMPLOYEES e2 where e.DEPARTMENT_ID=e2.DEPARTMENT_ID and e.DEPARTMENT_ID is not null)>
    (select j.max_salary from jobs j where j.job_id=e.JOB_ID)) 
    then (select j.max_salary from jobs j where j.job_id=e.JOB_ID)
    else  e.SALARY + (NVL(e.COMMISSION_PCT,0) * e.SALARY) + 0.1 * (select j.min_salary from jobs j
    where j.job_id=e.JOB_ID) + 0.1 * (select avg(e2.salary) from EMPLOYEES e2 where e.DEPARTMENT_ID=e2.DEPARTMENT_ID and e.DEPARTMENT_ID is not null)
    end
    where 12 <= MONTHS_BETWEEN(newsewt ,e.hire_date);
    end;
    /

execute UPDATE_SALARY;

SELECT e.employee_id, e.salary as Old_Salary, ec.salary as New_Salary,
j.min_salary, j.max_salary, e.hire_date
FROM employees_copy ec join employees e
on ec.employee_id=e.employee_id
join jobs j on e.job_id=j.job_id;





-- Question 3.
-- Create a trigger that activates when an employee’s salary is updated.
-- Salary decrease>20% → demotion
-- 1 new table: Demotions
-- (fields:
-- employee_id,
-- current_salary,
-- Status- waiting and done,
-- date
-- )
-- In case of demotion:
-- 1. No change if that employee is not a manager
-- 2. If that employee is a manager, switch him/her with the highest-paid employee under him/her. (Do
-- not switch salaries)

CREATE OR REPLACE TRIGGER salary_activation 
AFTER UPDATE OF salary ON employees
FOR EACH ROW
DECLARE
    v_is_manager NUMBER;
    v_highest_sub_id NUMBER;
BEGIN
    -- 1. Check for demotion (Decrease > 20%)
    IF ((:old.salary - :new.salary) / :old.salary > 0.2) THEN
        
        -- Insert into demotion table
        INSERT INTO demotions (employee_id, current_salary, status, demotion_date)
        VALUES (:old.employee_id, :new.salary, 'done', SYSDATE);

        -- 2. Check if the employee is a manager
        SELECT COUNT(*) INTO v_is_manager FROM employees WHERE manager_id = :old.employee_id;

        IF v_is_manager > 0 THEN
            -- 3. Find the highest-paid subordinate
            -- We use a subquery to find the ID of the max salary earner under this manager
            SELECT employee_id INTO v_highest_sub_id
            FROM (SELECT employee_id FROM employees 
                  WHERE manager_id = :old.employee_id 
                  ORDER BY salary DESC)
            WHERE ROWNUM = 1;

            -- 4. THE SWITCH:
            -- A. All other subordinates now report to the NEW manager
            UPDATE employees 
            SET manager_id = v_highest_sub_id 
            WHERE manager_id = :old.employee_id 
              AND employee_id <> v_highest_sub_id;

            -- B. The NEW manager now reports to whoever the OLD manager reported to
            UPDATE employees
            SET manager_id = :old.manager_id
            WHERE employee_id = v_highest_sub_id;

            -- C. The OLD manager now reports to the NEW manager (Demotion complete)
            -- Note: We can't update :NEW.manager_id in an AFTER trigger. 
            -- This usually requires a separate UPDATE statement or a BEFORE trigger.
        END IF;
    END IF;
END;
/