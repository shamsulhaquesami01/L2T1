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
select * from JOB_HISTORY;
SELECT * from DEPARTMENTS;
select * from JOBS;
SELECT * from EMPLOYEES;
select * from LOCATIONS;
-- Find employees who are either managers of more than one department (by being the manager of
-- employees from multiple departments) or who have a salary greater than the average salary of all
-- employees, or both. Exclude those who work in departments located in 'Seattle'. Additionally,
-- print a type for each employee as either “Multi-Dept Manager” or “Above Avg Salary” (for
-- employees satisfying both the criteria, printing either of the types will suffice).
select e.EMPLOYEE_ID,e.FIRST_NAME from EMPLOYEES e
where 2 <= (
    select count(distinct e2.department_id)
    from employees e2
    where e2.MANAGER_ID = e.EMPLOYEE_ID
) OR
e.SALARY > (SELECT avg(e3.salary) from EMPLOYEES e3 )
MINUS
(
    select distinct e3.EMPLOYEE_ID, e3.FIRST_NAME  from EMPLOYEES e3
    join DEPARTMENTS d on d.DEPARTMENT_ID=e3.DEPARTMENT_ID
    join LOCATIONS l on l.LOCATION_ID = d.LOCATION_ID
    where l.CITY='Seattle'
);



SELECT e.employee_id, 
       e.first_name || ' ' || e.last_name AS fullname,
       CASE 
           WHEN (SELECT COUNT(DISTINCT e2.department_id) 
                 FROM employees e2 
                 WHERE e2.manager_id = e.employee_id) >= 2 THEN 'Multi-Dept Manager'
           WHEN e.salary > (SELECT AVG(salary) FROM employees) THEN 'Above Avg Salary'
       END AS emp_type
FROM employees e
JOIN departments d ON e.department_id = d.department_id
JOIN locations l ON d.location_id = l.location_id
WHERE l.city <> 'Seattle' -- Seattle Exclusion
  AND (
      -- Condition 1: Multi-Dept Manager
      (SELECT COUNT(DISTINCT e2.department_id) 
       FROM employees e2 
       WHERE e2.manager_id = e.employee_id) >= 2 
      OR 
      -- Condition 2: Above Average Salary
      e.salary > (SELECT AVG(salary) FROM employees)
  );


--   For each department, show the department_name, employee_id, and salary of the highest-paid
-- employee(s). If a department has no employees, do not include it in the results. Display the output
-- in descending order of the salary. If two employees have the same salary, prioritize the one whose
-- department name is lexicographically larger. If there still exists a tie, prioritize the employee with lower employee_id.

select department_name,employee_id,salary 
from DEPARTMENTS d
join EMPLOYEES e on d.DEPARTMENT_ID=e.DEPARTMENT_ID
where  e.SALARY> ANY(
    select salary from EMPLOYEES e2 where e2.DEPARTMENT_ID = e.DEPARTMENT_ID
) AND 1 <= 
(select count(distinct e3.employee_id) 
from Employees e3 
where e3.DEPARTMENT_ID = e.DEPARTMENT_ID
)
order by e.salary asc,d.DEPARTMENT_NAME desc,e.EMPLOYEE_ID asc;


--------------

-- List departments where every employee has at least one job change, and display the department
-- name and the number of employees. Use a CASE statement to show if the department’s average
-- salary is ’Above’ or ‘Below or equal to’ the company average.


select department_name, count(DISTINCT e.EMPLOYEE_ID) as NUM_EMPLYES,
case
    when avg(e.SALARY) > (SELECT AVG(salary) FROM employees) THEN 'Above Company Average'
    ELSE 'Below or Equal to Company Average'
end as status
from EMPLOYEES e 
join JOB_HISTORY j on j.EMPLOYEE_ID=e.EMPLOYEE_ID
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
where e.JOB_ID <> j.JOB_ID
group by department_name;

-- Find employees who are either among the top 3 highest-earning employees in their department or
-- have a job title not held by anyone else in their department, or both. Exclude those who work in a
-- department with a manager whose first name is 'Steven'. Additionally, print a type for each
-- employee as either “Top 3 Salary” or “Unique Job” (for employees satisfying both the criteria,
-- printing either of the types will suffice).

select employee_id,first_name,last_name,
case
   when 2>= (select count(e2.employee_id) from EMPLOYEES e2 where e2.SALARY > e.SALARY)
   then 'Top 3 Salary'
   else 'Unique Job'  
end as TYPE
 from EMPLOYEES e
where 2>= (select count(e2.employee_id)
 from EMPLOYEES e2
 where e2.SALARY > e.SALARY
 ) OR
 e.JOB_ID <> ANY(
    select e3.JOB_ID
 from EMPLOYEES e3
 ) AND 'Steven' <> (select e5.first_name from EMPLOYEES e5 where e5.EMPLOYEE_ID=e.MANAGER_ID);



-- List the department_name, city, and country_name for departments that have no employees hired
-- before July 1, 2000, have a manager, and the manager’s job is not titled 'Stock Manager'. Display
-- only departments that have at least one employee in ascending order of country and descending
-- order of city. If there still exists a tie, display the lexicographically smaller department_name first.

select distinct d.department_name,l.city,c.country_name
from DEPARTMENTS d
join LOCATIONS l on d.LOCATION_ID=l.LOCATION_ID
join COUNTRIES c on c.COUNTRY_ID =l.COUNTRY_ID
join EMPLOYEES e on e.department_id=d.department_id
where e.hire_date >='01-JULY-2000' and
e.manager_id is not null and e.EMPLOYEE_ID is not null
order by c.country_name asc,l.city desc, d.department_name asc;


select * from JOB_HISTORY;
SELECT * from DEPARTMENTS;
select * from JOBS;
select * from JOB_HISTORY;
SELECT * from EMPLOYEES;
select * from LOCATIONS;

-- Find all employees who have a job title with a minimum salary greater than the average minimum
-- salary of all jobs, and work in departments that have more than 5 employees.

select employee_id,first_name, last_name,e.job_id,min_salary,department_name
from jobs j
join employees e on e.JOB_ID =j.JOB_ID 
join departments d on e.department_id = d.DEPARTMENT_ID
where 
 5 <= (
    select count(e2.employee_id) from EMPLOYEES e2 where e2.DEPARTMENT_ID=e.DEPARTMENT_ID
) and
j.MIN_SALARY > (
    select avg(j2.min_salary) from jobs j2
) ;


-- For each department, calculate its total salary expense. Categorize this expense as 'Low' (total
-- salary ≤ 15000), 'Medium' (total salary > 15000 and ≤ 50000), 'High' (total salary > 50000 and ≤
-- 100000), or 'Very High' (total salary > 100000). Display department_name, total_salary_expense,
-- and expense_category in descending order of the total salary expense and ascending order of
-- department_name. Include departments with no employees (their total expense should be 0 and
-- categorized as 'Low').

select department_name, sum(salary) as total_expense,
case
    when sum(salary) <=15000 then 'Low'
    when sum(salary) between 15001 and 50000 then 'Medium'
    when sum(salary) between 50001 and 100000 then 'High'
    else 'Very High'

end as expense_category
from DEPARTMENTS d
join EMPLOYEES e on e.DEPARTMENT_ID=d.DEPARTMENT_ID
group by d.DEPARTMENT_ID,d.DEPARTMENT_NAME;