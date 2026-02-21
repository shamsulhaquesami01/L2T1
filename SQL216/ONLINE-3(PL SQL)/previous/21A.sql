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

-- Write a Procedure named RANK JOBS that takes one integer MIN HIRED COUNT as input. It
-- ranks the jobs whose JOB
-- ID matches with the ID extracted from its JOB
-- TITLE. The extracted-- ID contains the first two letters of the first word of the title, followed by an underscore, followed
-- by the first three letters of the second word (if any) of the title. For example, from the title
-- ‘Sales Representative  the extracted ID would be ‘SA
-- REP’ If the actual JOB
-- ID also is ‘SA REP’ then there is a match.
-- The jobs having this match, which also have to have their number of hired employees no less
-- than MIN HIRED COUNT, are then ranked by the Procedure RANK JOBS. The ranking is done according to the descending order of
-- their average salaries, with the job having the highest average salary ranked as 1. Finally, insert the ranks of the jobs in the JOB
-- RANK table given below.

CREATE OR REPLACE PROCEDURE RANK_JOBS(Min_hired_count in number ) IS
BEGIN
  
    FOR R IN (
        SELECT job_title, emp_count, min_salary, max_salary, avg_salary,
               RANK() OVER (ORDER BY avg_salary DESC) as rnk
        FROM (
            -- Subquery to compute counts/avgs so the aliases are available for RANK()
            SELECT J.Job_title, 
                   (SELECT COUNT(*) FROM employees e2 WHERE e2.JOB_ID = j.JOB_ID) as emp_count, 
                   j.min_salary, 
                   j.max_salary, 
                   (SELECT AVG(e2.salary) FROM employees e2 WHERE e2.JOB_ID = j.JOB_ID) as avg_salary
            FROM JOBS J
            where (SELECT COUNT(*) FROM employees e2 WHERE e2.JOB_ID = j.JOB_ID) > Min_hired_count and
            JOB_ID = (SUBSTR(JOB_TITLE, 1, 2) || '_' || SUBSTR(REGEXP_SUBSTR(JOB_TITLE, '\S+', 1, 2), 1, 3))
        )
    )
    LOOP
        -- Access fields via the record variable 'R'
        DBMS_OUTPUT.PUT_LINE('rank= ' || R.rnk || ' | job = ' || R.job_title || ' | emp count = ' || R.emp_count);
    END LOOP;
END;
/

execute RANK_JOBS(1);