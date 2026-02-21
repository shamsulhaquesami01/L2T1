-- Please write a trigger to validate updates of salary values in the TEMP_EMPLOYEES table,
-- which is a copy of the HR.EMPLOYEES table). The trigger should only be invoked when the
-- SALARY column is updated in the TEMP_EMPLOYEES Table.

-- This trigger should check if the updates are valid. The constraint for validating salary of an
-- employee is as follows:
-- ● Case 1: The salary cannot be less than the minimum salary of the same JOB_ID of other
-- employees in the company.
-- ● Case 2: If the employee is not a manager, then the salary must be less than the
-- minimum salaries of the managers who manage the same JOB_ID.
-- ● Case 3: If the employee is a manager, the salary must be less than the salary of the
-- manager of the same department.
-- If the salary is not valid, then the trigger should throw an exception so that the update results in
-- an error.
-- If the salary is valid, the update should be allowed.
-- First, create a TEMP_EMPLOYEES table that is a copy of the EMPLOYEES table (SQL given
-- below). Then write your trigger on the test TEMP_EMPLOYEES table. To avoid the “mutating”
-- trigger issue, all your PL/SQL statements should perform validation checks on the original
-- EMPLOYEES table (instead of TEMP_EMPLOYEES table).






DROP TABLE TEMP_EMPLOYEES;
CREATE TABLE TEMP_EMPLOYEES AS SELECT * FROM EMPLOYEES;
SELECT * FROM TEMP_EMPLOYEES;

create or REPLACE trigger validity
before UPDATE
of salary
on TEMP_EMPLOYEES
DECLARE
count number;
job_min_sal number;
sub_emp_count number;
manager_same_dept_salary number;
BEGIN
    count :=0;
    select j.min_salary into job_min_sal from jobs j where j.JOB_ID=:old.job_id;
    if(:new.salary>=job_min_sal) then count := count+1;
    end if;
    select count(e.employee_id) into sub_emp_count from employees e where e.manager_id=:old.employee_id;
    select e.salary into manager_same_dept_salary from EMPLOYEES e 
    where e.EMPLOYEE_ID=(select d.manager_id  from DEPARTMENTS d where d.DEPARTMENT_ID=:old.DEPARTMENT_ID);
    if(sub_emp_count>0)
    then if(:old.salary<manager_same_dept_salary) then count := count+1;
         end if;
    ELSE
        DBMS_OUTPUT.PUT_LINE('i dont know');
    end if;
    if(count <> 2) then RAISE_APPLICATION_ERROR(-20101, 'violated constraints');
    end if;
    end;
    /

