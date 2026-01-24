

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

--*******-
-- Find the employees who get the second highest salary in their respective department. Show
-- their first name, last name, department name, salary. The output should be like the table below:

select first_name, last_name,d.department_name, Salary
from EMPLOYEES e
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
 WHERE
1 = (
    select count(e2.salary) from EMPLOYEES e2
    where e2.SALARY > e.SALARY and e2.DEPARTMENT_ID=e.DEPARTMENT_ID
);

select e.first_name,
       e.last_name,
       d.department_name,
       e.salary
from employees e
join departments d
  on d.department_id = e.department_id
where e.salary = (
    select max(e2.salary)
    from employees e2
    where e2.department_id = e.department_id
      and e2.salary < (
          select max(e3.salary)
          from employees e3
          where e3.department_id = e.department_id
      )
);

--*******-
-- For each employee, find out the number of unique posts he has served so far, including his
-- current job. Sort the result by employee id. You cannot use any AND, OR, NOT operators. Use
-- set operations instead. Sample output:
select employee_id,count(distinct numbers) from
(select employee_id, job_id as numbers from EMPLOYEES
UNION
select employee_id, job_id  as numbers from JOB_HISTORY)
group by employee_id
order by employee_id;

--****
-- Print information of the employees that get the highest salary in his own department. Use join
-- (50% marks will be deducted if done using subquery). Order the result by employee id. Hint:
-- while joining with other employees with higher salary, an employee with the highest salary will
-- not find any other employee to join with.

select e1.employee_id,
       e1.first_name,
       e1.last_name,
       e1.salary,
       e1.department_id
from employees e1
left join employees e2
  on e1.department_id = e2.department_id
  and e1.salary < e2.salary  
where e2.employee_id is null  
order by e1.employee_id;

-- Print information of the employees that receive the highest salary under their managers. Order
-- the results by employee id. Use subquery (50% marks will be deducted if done using join).
select * from EMPLOYEES e
where e.SALARY = (
    select max(e2.salary) from EMPLOYEES e2
    where e2.MANAGER_ID=e.MANAGER_ID
)
order by e.employee_id;

-- For each country, find the department with the highest salaried manager among the managers
-- of all the departments located in that country. For example, consider the following one row of
-- sample output. It implies that the manager of the Executive department has the highest salary
-- among all the managers of the departments located in the United States of America. Sort the
-- result by the country name.
select c.country_name, d.department_name, e.salary
from departments d
join locations l on l.location_id = d.location_id
join countries c on c.country_id = l.country_id
join employees e on e.employee_id = d.manager_id
where e.salary = (
    select max(e2.salary)
    from departments d2
    join locations l2 on l2.location_id = d2.location_id
    join employees e2 on e2.employee_id = d2.manager_id
    where l2.country_id = l.country_id
)
order by c.country_name;

-- Write a query to retrieve the employees who have a salary greater than the average salary in
-- their respective department.
-- Show their first name, last name, department name, salary and average salary of the
-- department.

select e.first_name, e.last_name,d.department_name,e.salary, (
    select round(avg(e3.salary)) from employees e3
    where e3.DEPARTMENT_ID=e.DEPARTMENT_ID
) as avg
from EMPLOYEES e
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
 where salary >(
    select avg(e2.salary) from EMPLOYEES e2
    where e2.DEPARTMENT_ID=e.department_id
);

select e.first_name, e.last_name, d.department_name, e.salary, round(dep_avg.avg_salary) as avg
from employees e
join departments d on d.department_id = e.department_id
join (
    select department_id, avg(salary) as avg_salary
    from employees
    group by department_id
) dep_avg on dep_avg.department_id = e.department_id
where e.salary > dep_avg.avg_salary;

-- FInd out employees who are not managers and who do not have any prior experiences
-- (experiences are stored in job history). Print the employee ids only. You cannot use any NOT
-- operator. Use set operations instead.

SELECT EMPLOYEE_ID
FROM EMPLOYEES
MINUS
SELECT MANAGER_ID
FROM EMPLOYEES
WHERE NVL(MANAGER_ID, -1) > -1
MINUS
SELECT EMPLOYEE_ID
FROM JOB_HISTORY;

-- For all jobs, print the job id, job title and the number of managers (employees that manage other
-- employees) doing that job. Order the result by job id. Use join (50% marks will be deducted if
-- done using subquery).
select * from jobs;
select * from EMPLOYEES;

select j.job_id, j.job_title,
(select count(e.employee_id) from EMPLOYEES e
where j.job_id=e.job_id AND
1<=(
    select count(e2.employee_id) from EMPLOYEES e2
    where e2.MANAGER_ID=e.EMPLOYEE_ID
)
 )
from jobs j;



-- For each department, find the ratio of the number of employees receiving above or equal to
-- average salary and the number of employees receiving below or equal to average salary in that
-- department. For example, consider the following one row of sample output. It shows that the
-- Purchasing department has the average salary 4150, and the number of employees receiving

-- salary below or equal to 4150 (the average salary of the purchasing department) is five times
-- the number of employees receiving salary above or equal to 4150. In the output, omit the
-- departments which do not have any employees. Sort the result by department id.

select d.department_id,d.department_name,(
    select avg(salary) from EMPLOYEES e
    where e.department_id=d.department_id
    ),(
        (select count(*) from employees e1
        where e1.SALARY >= (
    select avg(salary) from EMPLOYEES e2
    where e1.department_id=e2.department_id
    ) and e1.department_id=d.DEPARTMENT_ID )
 /
nullif((select count(*) from employees e1
        where e1.SALARY <= (
    select avg(salary) from EMPLOYEES e2
    where e1.department_id=e2.department_id
    )and e1.department_id=d.DEPARTMENT_ID ),0)

       )   as ratio
from DEPARTMENTS d;

select d.department_id,
       d.department_name,
       (
           select avg(salary)
           from employees e
           where e.department_id = d.department_id
       ) as avg_sal,
       (
           (select count(*)
            from employees e1
            where e1.salary >= (
                select avg(salary)
                from employees e2
                where e2.department_id = e1.department_id
            )
            and e1.department_id = d.department_id
           ) * 1.0
           /
           (select count(*)
            from employees e1
            where e1.salary <= (
                select avg(salary)
                from employees e2
                where e2.department_id = e1.department_id
            )
            and e1.department_id = d.department_id
           )
       ) as ratio
from departments d
where exists (
    select 1
    from employees e
    where e.department_id = d.department_id
)
order by d.department_id;
