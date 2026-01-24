

select * from JOB_HISTORY;
SELECT * from DEPARTMENTS;
select * from JOBS;
select * from JOB_HISTORY;
SELECT * from EMPLOYEES;
select * from LOCATIONS;
select * from REGIONS;

-- Print information of all the managers (employees that manage other employees) whose
-- managed employees do the same job as him/her. Use join (50% marks will be deducted if done
-- using subquery).
select DISTINCT m.* from EMPLOYEES m 
join EMPLOYEES e on m.EMPLOYEE_ID = e.MANAGER_ID
where m.JOB_ID = e.JOB_ID;

-- Print information of the employees that are the only employees in their job. Order the results by
-- employee id. Use subquery (50% marks will be deducted if done using join).

select * from employees e
where 1 = (select count(e2.employee_id) from employees e2 where e2.job_id=e.job_id)
order by e.EMPLOYEE_ID;

-- Find all the departments that have at least one employee who was hired before the manager of
-- the department. For example, consider the following one row of sample output. It implies that
-- the shipping department has at least one employee who was hired before the manager of the
-- shipping department. Sort the result by department id.

select d.DEPARTMENT_ID, d.DEPARTMENT_NAME from DEPARTMENTS d
join EMPLOYEES m on d.MANAGER_ID=m.EMPLOYEE_ID
where 1 <=(
    select count(e2.employee_id) from EMPLOYEES e2
    where e2.DEPARTMENT_ID=d.DEPARTMENT_ID
    and e2.HIRE_DATE < m.HIRE_DATE and e2.EMPLOYEE_ID <> m.EMPLOYEE_ID
)
order by d.DEPARTMENT_ID;
