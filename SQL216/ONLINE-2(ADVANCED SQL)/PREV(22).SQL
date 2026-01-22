-- Find employees who are either in departments with more than 5 employees or have a job title
-- with a minimum salary above 10000, or both. Exclude those in departments where the manager
-- earns less than their department's average.

select e.employee_id,
       e.first_name,
       e.last_name,
       'Large Dept' as type
  from employees e
 where e.department_id in (
   select department_id
     from employees
    group by department_id
   having count(*) > 5
)
union
-- Employees with a job title with minimum salary above 10000
select e.employee_id,
       e.first_name,
       e.last_name,
       'High Min Job' as type
  from employees e
  join jobs j
on e.job_id = j.job_id
 where j.min_salary > 10000
minus
-- Exclude employees in departments where the manager earnsless than department average
select e.employee_id,
       e.first_name,
       e.last_name,
       '' as type
  from employees e
  join departments d
on e.department_id = d.department_id
  join employees m
on d.manager_id = m.employee_id
 where m.salary < (
   select avg(salary)
     from employees
    where department_id = m.department_id
);


-- For each country, count the number of departments. Display only the country_name and
-- department_count, in ascending order of the country_name. Include the countries having no
-- departments, too.

select * from COUNTRIES; 
SELECT * from DEPARTMENTS;
SELECT * from LOCATIONS;

select country_name, count(d.DEPARTMENT_ID) departments
from COUNTRIES c
JOIN LOCATIONS L
on c.COUNTRY_ID=L.COUNTRY_ID
JOIN DEPARTMENTS d
ON L.LOCATION_ID= d.LOCATION_ID
GROUP by country_name
ORDER by country_name;


-- For each department, find the employee_id, full_name, salary, department_name, and job title of
-- the second-highest-paid employee(s) i.e. employee(s) having the second-highest salary. If a
-- department has fewer than two employees, do not include it in the results. Display the output in
-- descending order of the salary. If two employees have the same salary, prioritize the one whose
-- department name is lexicographically smaller. If a tie still exists, prioritize the employee with the
-- lower employee_id.

SELECT * from DEPARTMENTS;
SELECT * from EMPLOYEES;
select * from LOCATIONS;


SELECT E.EMPLOYEE_ID, 
       E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME, 
       E.SALARY, 
       D.DEPARTMENT_NAME, 
       J.JOB_TITLE
FROM EMPLOYEES E
JOIN DEPARTMENTS D ON (E.DEPARTMENT_ID = D.DEPARTMENT_ID)
JOIN JOBS J ON (E.JOB_ID = J.JOB_ID)
WHERE 1 = (
    -- The Correlated Sub-query: Count how many people in the SAME department
    -- have a higher salary than the current employee (E)
    SELECT COUNT(DISTINCT E2.SALARY)
    FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID = E.DEPARTMENT_ID
      AND E2.SALARY > E.SALARY
)
ORDER BY E.SALARY DESC, D.DEPARTMENT_NAME ASC, E.EMPLOYEE_ID ASC;

-- Find the employee_id, first_name, and salary of employees in descending order of the salary and
-- ascending order of the employee ID, who meet exactly one of the following two conditions:
-- a. They report to a manager whose salary is greater than 15000.
-- b. They work in a department located in 'Seattle'.

select employee_id,first_name,salary
from EMPLOYEES e
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
join LOCATIONS l on l.LOCATION_ID = d.LOCATION_ID
where 
(
        e.MANAGER_ID IN (SELECT employee_id FROM EMPLOYEES WHERE salary > 15000)
        OR l.CITY = 'Seattle'
    )
AND NOT
(l.CITY= 'Seattle' AND e.MANAGER_ID in (
    SELECT e2.EMPLOYEE_ID 
    from EMPLOYEES e2
    where e2.SALARY >15000
))
ORDER by e.SALARY desc, e.EMPLOYEE_ID asc;

SELECT * from DEPARTMENTS;
select * from JOBS;
SELECT * from EMPLOYEES;
select * from LOCATIONS;

-- Find employees (first and last name), their departments, and salary, for those who earn more than
-- the average salary in their own department. Only consider departments where there is at least one
-- employee earning less than the company average salary and at least one earning more than the
-- company average salary. Use a CASE statement to categorize salary as 'High' (if above 10,000),
-- 'Medium' (if between 5,000 and 10,000), or 'Low' (if below 5,000).



SELECT first_name||last_name as fullname , department_name, salary
from EMPLOYEES e
join DEPARTMENTS d on e.DEPARTMENT_ID = d.DEPARTMENT_ID
join (
    select DEPARTMENT_ID,avg(salary) as average
    from EMPLOYEES e2
    GROUP by DEPARTMENT_ID
) X on X.DEPARTMENT_ID= e.DEPARTMENT_ID
AND e.department_id IN (
    -- Sub-query 2: Departments meeting the Company Average constraints
    SELECT department_id
    FROM employees
    GROUP BY department_id
    HAVING MIN(salary) < (SELECT AVG(salary) FROM employees) -- At least one below company avg
       AND MAX(salary) > (SELECT AVG(salary) FROM employees) -- At least one above company avg
);



-----------

-- Find all employees who earn more than the salary of at least five distinct employees who work in
-- a different department but are located in the same city as the first employee's department. Display
-- the employee_id, full_name, department_name, and salary of these employees, in descending
-- order of their department_name. If the department name is the same, break the tie by choosing the
-- employee with more salary and lower employee_id.

select employee_id, first_name||last_name as fullname,department_name,salary
from EMPLOYEES e 
join DEPARTMENTS D ON e.department_id = d.department_id
JOIN locations l ON d.location_id = l.location_id 
where 5 <= (
    select count(e2.employee_id) from EMPLOYEES e2
    join DEPARTMENTS d2 on d2.DEPARTMENT_ID=e2.DEPARTMENT_ID
    join LOCATIONS l2 on l2.LOCATION_ID = d2.LOCATION_ID
    where e2.SALARY <e.SALARY AND e2.DEPARTMENT_ID <> e.DEPARTMENT_ID AND l2.CITY = l.CITY
)
order by D.DEPARTMENT_NAME desc, e.SALARY desc, e.EMPLOYEE_ID asc;


-- Find the departments located in Americas that have a department manager whose salary is
-- between 10000 and 15000 (inclusive), and manages at least two employees. Do not include the
-- departments which has no manager. Show only the department’s name, the manager’s full name,
-- salary of the manager, in descending order of the manager’s salary and ascending order of the
-- department name.
select department_name,first_name||last_name as fullname,salary
from departments d 
join EMPLOYEES e on d.MANAGER_ID=e.EMPLOYEE_ID
join locations l on l.LOCATION_ID=d.LOCATION_ID
join countries c on c.COUNTRY_ID=l.COUNTRY_ID
join regions r on r.REGION_ID=c.REGION_ID
where r.REGION_NAME='Americas' AND e.SALARY between 10000 and 15000 AND d.MANAGER_ID is not null AND
2 <= (select count(distinct employee_id) 
        from EMPLOYEES e2
        where e2.MANAGER_ID = d.MANAGER_ID
)
order by e.salary desc,d.DEPARTMENT_NAME asc;


-- List departments where at least one employee has held every job title currently assigned to
-- employees in that department (based on job history). For each such department, display the
-- department name and a CASE statement indicating whether the department’s average salary is
-- ‘Above’ or ‘Below or equal’ to the company average.
