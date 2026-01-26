-- 1. Find employees earning above their department's average salary in departments with more than 4
-- employees.

SELECT E.EMPLOYEE_ID FROM EMPLOYEES E 
WHERE E.SALARY > (
    SELECT AVG(E2.SALARY) FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID=E.DEPARTMENT_ID
) AND 4 < (
    SELECT COUNT(E3.EMPLOYEE_ID) FROM Employees E3
    WHERE E3.DEPARTMENT_ID=E.DEPARTMENT_ID
);

-- 2. Find employees who either earn more than their manager's salary or have a salary greater than
-- their department's average salary. Print employee details with the type as either "Higher Than
-- Manager" or "Above Dept Avg"

SELECT E.EMPLOYEE_ID,
CASE
WHEN 
E.SALARY >(
    SELECT E2.SALARY FROM EMPLOYEES E2
    WHERE E2.EMPLOYEE_ID=E.MANAGER_ID
) THEN 'Higher Than Manager'
ELSE 'ABOVE AVG DEPT'
END AS TYPE
FROM EMPLOYEES E 
WHERE E.SALARY >(
    SELECT E2.SALARY FROM EMPLOYEES E2
    WHERE E2.EMPLOYEE_ID=E.MANAGER_ID
) OR E.SALARY > (
    SELECT round(AVG(E2.SALARY)) FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID=E.DEPARTMENT_ID
);

-- 3. Write a SQL query for employees whose salary beats their department average and whose
-- manager's salary beats the company average. Show full
-- _
-- name, salary, department
-- name, and
-- _
-- label it 'Dept Top Earner' if salary > 1.5 times dept average, else 'Dept Above Avg'
-- .

SELECT E.FIRST_NAME, E.SALARY,D.department_name,
CASE
WHEN E.SALARY > 1.5*(
    SELECT AVG(E2.SALARY) FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID=E.DEPARTMENT_ID
) THEN 'Dept Top Earner'
ELSE 'Dept Above Avg'
END as label
 FROM EMPLOYEES E 
 JOIN DEPARTMENTS D ON D.DEPARTMENT_ID=E.DEPARTMENT_ID
WHERE E.SALARY > (
    SELECT AVG(E2.SALARY) FROM EMPLOYEES E2
    WHERE E2.DEPARTMENT_ID=E.DEPARTMENT_ID
) AND (
    SELECT E3.SALARY FROM EMPLOYEES E3
    WHERE E3.EMPLOYEE_ID=E.MANAGER_ID
) > (SELECT AVG(E4.SALARY) FROM EMPLOYEES E4);



-- 4. Find employee
-- _
-- id, full name, and department name of employees whose department is located in
-- the same city as their manager’s department.

select e.employee_id, d.department_name,e.first_name
from Employees e
join departments d on d.department_id=e.department_id
join locations l on l.LOCATION_ID=d.LOCATION_ID
where l.city = (
select l2.city from locations l2
join departments d2 on l2.LOCATION_ID=d2.LOCATION_ID
join Employees e2 on d2.department_id=e2.department_id
where e2.employee_id=e.MANAGER_ID
);

--  Write an SQL query to list all departments that satisfy the following conditions:
-- (i) every employee in the department earns more than 5000,
-- (ii) the department has at least one employee with job history, and
-- (iii) the maximum salary in the department is greater than the overall company average salary.
-- For each such department, display the department name, number of employees, average salary, and a
-- column called Salary_
-- Level that shows
-- ●
-- ●
-- 'Elite' if the department’s average salary is greater than 1.5 times the company average salary,
-- 'Above Average' otherwise.

select d.department_name from departments d 
where 5000<all(
    select e.salary from employees e
    where e.department_id=d.department_id
    ) 
    AND
        (select max(e3.SALARY) from EMPLOYEES e3
        where e3.department_id=d.department_id)
        >(
            select avg(salary) from Employees
        ) 
        AND
        1<=(
            select count(e4.employee_id) from Employees e4
            where e4.employee_id in(
                select j.employee_id from JOB_HISTORY j
            ) and e4.department_id=d.department_id
        ) ;
