-- •	Employees who satisfy exactly one:
	-- •	dept has ≥ 4 employees
	-- •	salary > department average

select *
  from employees e
 where ( 4 <= (
   select count(employee_id)
     from employees
    where e.department_id = department_id
)
    or salary > (
   select avg(salary)
     from employees
    where department_id = e.department_id
) )
   and not ( 4 <= (
      select count(employee_id)
        from employees
       where e.department_id = department_id
   )
   and salary > (
   select avg(salary)
     from employees
    where department_id = e.department_id
) );

-- Employees whose salary is:
-- 	•	greater than their department average
-- 	•	but less than their manager’s salary

-- Display: name, salary, dept_id, manager_salary
select e.first_name, e.salary as salary, e.department_id,m.salary as manager_salary
from EMPLOYEES e 
join EMPLOYEES m on e.MANAGER_ID=m.EMPLOYEE_ID
where e.SALARY>(
    select avg(salary) from EMPLOYEES e2
    where e2.department_id=e.DEPARTMENT_ID
) AND e.SALARY <m.SALARY;

-- Departments where:
-- 	•	avg salary > company avg
-- 	•	AND have at least one employee with no commission

select d.department_name from DEPARTMENTS d
where (
    select avg(salary) from EMPLOYEES e
    where e.DEPARTMENT_ID=d.DEPARTMENT_ID
) > (
    select avg(salary) from EMPLOYEES e2
) AND 1 <= (
    select count(employee_id) from EMPLOYEES e3
    where e3.COMMISSION_PCT is NULL
);


-- Employees who:
-- 	•	have never changed jobs
-- 	•	work in departments with a manager

select * from EMPLOYEES e
where not EXISTS (
    select j.EMPLOYEE_ID from JOB_HISTORY j
    where j.employee_id = e.EMPLOYEE_ID
) AND e.MANAGER_ID is not NULL;

-- Departments that:
-- 	•	have employees
-- 	•	but no job history records at all

SELECT d.department_name from DEPARTMENTS d 
where exists (
    select e.employee_id from EMPLOYEES e 
    where e.DEPARTMENT_ID=d.DEPARTMENT_ID
) and not exists(
    select j.EMPLOYEE_ID from JOB_HISTORY j
    where j.DEPARTMENT_ID = d.DEPARTMENT_ID
);

-- Jobs where:
-- 	•	max salary – min salary > 12000
-- 	•	and avg salary > company avg

select * from jobs;

select j.JOB_TITLE from jobs j
where j.MAX_salary-j.min_salary >12000 AND
(select avg(salary) from EMPLOYEES where job_id=j.JOB_ID) >
(select avg(salary) from EMPLOYEES );

-- Employees labeled as:
-- 	•	'Elite' → salary ≥ 2× dept avg
-- 	•	'Above Avg' → salary > dept avg
-- 	•	'Normal' → otherwise

SELECT e.first_name,
case
when e.salary >= 2*(select avg(salary) from EMPLOYEES where DEPARTMENT_ID=e.department_id) 
then 'Elite'
when e.salary > (select avg(salary) from EMPLOYEES where DEPARTMENT_ID=e.department_id)
then 'Above Avg'
else 'Normal'
end as label
 from EMPLOYEES e;

--  Employees who earn:
-- 	•	the minimum salary in their department
-- 	•	but not the minimum salary in the company


