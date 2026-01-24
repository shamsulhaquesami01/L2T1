-- List managers whose departments have average salaries higher than the overall company average,
-- for departments located in Toronto and Oxford

select distinct e2.FIRST_NAME||' '||e2.last_name as fullname from DEPARTMENTS d
join locations l on l.LOCATION_ID = d.LOCATION_ID
join EMPLOYEES e2 on e2.EMPLOYEE_ID=d.MANAGER_ID
where l.CITY = 'Toronto' OR l.CITY = 'Oxford' AND
(select avg(e3.salary) from EMPLOYEES e3
where e3.DEPARTMENT_ID=d.DEPARTMENT_ID
)> (select avg(e4.salary) from EMPLOYEES e4);

-- 2. Find employees who both work in departments with more than 5 employees AND have salaries
-- greater than the overall average salary across all employees.

select e.employee_id, e.FIRST_NAME||' '||e.LAST_NAME as fullname from EMPLOYEES e
where 5 < (
    select count(distinct e2.employee_id )from employees e2
    where e2.department_id=e.department_id
) AND e.SALARY > (
    select avg(e3.salary)  from EMPLOYEES e3

);

-- 3. Write a SQL query for employees in departments that have managers, with no job history records,
-- and salary > dept average. Show ful_name, salary, dept_name, and label 'Stable High Earner' if
-- salary > 1.7 times dept average, else 'Dept Above Avg'

select e.FIRST_NAME||' '||e.LAST_NAME as fullname ,e.salary, d.department_name,
case
when e.SALARY >1.7*(
    select avg(e2.salary) from EMPLOYEES e2
    where e2.DEPARTMENT_ID=e.DEPARTMENT_ID
) then 'Stable High Earner'
else 'Dept Above Avg'
end as label
from Employees e 
join departments d on d.department_id=e.department_id
where e.MANAGER_ID is not null AND 0=(
    select count(j.employee_id) from JOB_HISTORY j
    where j.EMPLOYEE_ID=e.EMPLOYEE_ID
) AND e.SALARY >(
    select avg(e2.salary) from EMPLOYEES e2
    where e2.DEPARTMENT_ID=e.DEPARTMENT_ID
);

-- Find employees who are either in departments with more than 5 employees or have a job with
-- minimum salary above 10000.
-- Display: employee_id, first_name,last_name,DEPARTMENT_ID,JOB_ID,SALARY

select e.employee_id, e.FIRST_NAME||' '||e.LAST_NAME as fullname ,e.DEPARTMENT_ID,
e.JOB_ID,e.SALARY
 from EMPLOYEES e
where 5 < (
    select count(distinct e2.employee_id )from employees e2
    where e2.department_id=e.department_id
) OR 10000<(
    select MIN_SALARY  from jobs j
    where j.JOB_ID=e.JOB_ID
);

-- 5. Write an SQL query to find employees who satisfy exactly one of the following conditions:
-- (i) they work in a department with more than 5 employees, or
-- (ii) their job has a minimum salary greater than 10000.
-- Employees who satisfy both conditions or neither condition must be excluded. Display employee
-- ID, full name, department ID, job ID, and salary

select e.employee_id,e.FIRST_NAME||' '||e.LAST_NAME as fullname,
e.department_id,e.job_id,e.salary from employees e 
where 5 < (
    select count(distinct e2.employee_id )from employees e2
    where e2.department_id=e.department_id
) OR 10000<(
    select MIN_SALARY  from jobs j
    where j.JOB_ID=e.JOB_ID
) AND NOT (
    5 < (
    select count(distinct e2.employee_id )from employees e2
    where e2.department_id=e.department_id
) AND 10000<(
    select MIN_SALARY  from jobs j
    where j.JOB_ID=e.JOB_ID
)
);
