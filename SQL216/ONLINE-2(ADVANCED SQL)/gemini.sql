-- Find the LAST_NAME and SALARY of employees who work in a city 
-- starting with 'S' and earn more than the maximum salary of any employee in the 'Sales' department.
--  Exclude employees who are currently managers
select e.last_name, e.salary ,l.CITY
from EMPLOYEES e
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
join LOCATIONS l on l.LOCATION_ID=d.LOCATION_ID
where l.CITY like 'S%' AND e.SALARY > (
    select max(e2.salary) from EMPLOYEES e2
    where e2.DEPARTMENT_ID = (
        select d2.DEPARTMENT_ID from DEPARTMENTS d2
        where d2.DEPARTMENT_NAME ='Sales'
    )
) AND 0 = (
    select count(DISTINCT e3.employee_id) from EMPLOYEES e3
    where e3.MANAGER_ID=e.EMPLOYEE_ID
);

-- List the DEPARTMENT_NAME and the number of employees for departments where 
-- the average salary is higher than the company average, 
-- but the minimum salary in that department is also higher 
-- than the average salary of departments located in 'Seattle'

SELECT d.department_name , count(e.employee_id)
from DEPARTMENTS d join EMPLOYEES e on e.DEPARTMENT_ID=d.DEPARTMENT_ID
where (select avg(e2.salary) from EMPLOYEES e2 where e2.DEPARTMENT_ID=d.DEPARTMENT_ID)>
(select avg(e3.salary) from EMPLOYEES e3) AND (select MIN(e4.salary) from EMPLOYEES e4 where e4.DEPARTMENT_ID=d.DEPARTMENT_ID)>
(select avg(e5.salary) from EMPLOYEES e5 where e5.DEPARTMENT_ID in (
    select d2.DEPARTMENT_ID from DEPARTMENTS d2
    where d2.LOCATION_ID in (
     select l.location_id from LOCATIONS l where l.CITY='Seattle'
    )
))
GROUP by d.DEPARTMENT_NAME;

select d.DEPARTMENT_NAME ,l.CITY
from EMPLOYEES e
join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
join LOCATIONS l on l.LOCATION_ID=d.LOCATION_ID;

-- The "XOR" Career Logic Find employees who satisfy exactly one of the following:

-- (i) They have held more than two different jobs in their career (check JOB_HISTORY).

-- (ii) Their current salary is at least 2.5 times the minimum salary defined for their JOB_ID. 
-- Exclude those working in departments with no manager assigned. Display: FULL_NAME, JOB_ID, and SALARY.




-- For each department, show the DEPARTMENT_NAME and a "Status" label:

-- 'Elite' if the department has at least one employee earning more than $15,000.

-- 'Balanced' if the department average is within +/- $1,000 of the global company average.

-- 'Under Review' for all other departments. Sort by DEPARTMENT_NAME ascending