-- f. For each employee, finds the total number of employees those were hired before him/her. Print
-- employee last name and total employees.
-- g. For each employee, finds the total number of employees those were hired before him/her and
-- those were hired after him/her. Print employee last name, total employees hired before him,
-- and total employees hired after him.
-- h. Find the employees having salaries greater than at least three other employees
-- i. For each employee, find his rank, i.e., position with respect to salary. The highest salaried
-- employee should get rank 1 and lowest salaried employee should get the last rank. Employees
-- with same salary should get same rank value. Print employee last names and his/he rank.
-- j. Finds the names of employees and their salaries for the top three highest salaried employees.
-- The number of employees in your output should be more than three if there are employees with
-- same salary


select e.last_name,(
    select count(*) from EMPLOYEES e2
    where e2.HIRE_DATE < e.hire_date
)as total from EMPLOYEES e;


select e.last_name,(
    select count(*) from EMPLOYEES e2
    where e2.HIRE_DATE < e.hire_date
)as total_before,(
   select count(*) from EMPLOYEES e2
    where e2.HIRE_DATE > e.hire_date

) as total_after from EMPLOYEES e;


select * from EMPLOYEES e
where 3 <= (
select count(*) from EMPLOYEES e2
where e2.SALARY<e.salary
);


select last_name,
       rank() over (order by salary desc) as salary_rank
from employees;

select e.last_name,
       (
         select count( e2.salary)
         from employees e2
         where e2.salary > e.salary
       ) + 1 as salary_rank
from employees e
order by salary_rank;


select e.last_name,e.salary from EMPLOYEES e
where 2 >=(
    select count(distinct e2.salary) from EMPLOYEES e2
    where e2.SALARY > e.SALARY
);



-----------------CHAPTER 06 (SUB QUERY)-------------------

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE SALARY >
(
SELECT SALARY
FROM EMPLOYEES
WHERE LAST_NAME = 'Abel' -- unique chilo lastname
) ;   --where e sub query; amonvabe dite hobe jeno ekta row return kore.


SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE JOB_ID =
(
SELECT JOB_ID
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 141  --this query returns one row
) ;  


SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE JOB_ID =
(
SELECT JOB_ID
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 141
)
AND SALARY >
(
SELECT SALARY
FROM EMPLOYEES
WHERE LAST_NAME = 'Abel'
) ;


SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE SALARY =
(
SELECT MAX(SALARY)  --still return one row
FROM EMPLOYEES
) ;


--any and all for multiple rows to make them single

SELECT LAST_NAME, JOB_ID, SALARY
FROM EMPLOYEES
WHERE JOB_ID <> 'IT_PROG'
AND SALARY < ANY
(
SELECT SALARY
FROM EMPLOYEES
WHERE JOB_ID = 'IT_PROG'
) ;


SELECT LAST_NAME, JOB_ID, SALARY
FROM EMPLOYEES
WHERE JOB_ID <> 'IT_PROG' AND SALARY < ALL
(
SELECT SALARY
FROM EMPLOYEES
WHERE JOB_ID = 'IT_PROG'
) ;



--------------------exercise 6.2----


--retrieve those employees whose salary is higher than at least three other employees
SELECT *
FROM EMPLOYEES E1
WHERE 3 <= (
SELECT COUNT(*)
FROM EMPLOYEES E2
WHERE E2.SALARY < E1.SALARY
) ;


SELECT DEPARTMENT_NAME
FROM DEPARTMENTS D
WHERE EXISTS
(
SELECT *
FROM EMPLOYEES E
WHERE E.DEPARTMENT_ID = D.DEPARTMENT_ID AND JOB_ID = 'IT_PROG'
) ;


--find those employees whose earns the maximum salary in his/her department
SELECT LAST_NAME, SALARY, DEPARTMENT_ID
FROM EMPLOYEES E1
WHERE NOT EXISTS
(
SELECT *
FROM EMPLOYEES E2
WHERE E2.DEPARTMENT_ID = E1.DEPARTMENT_ID AND
E2.SALARY > E1.SALARY
) ;

--same thing kintu select e sub query diye dept naame tao dekhano jabe
SELECT LAST_NAME, SALARY, (SELECT DEPARTMENT_NAME FROM DEPARTMENTS D
WHERE D.DEPARTMENT_ID = E1.DEPARTMENT_ID)
DEPARTMENT
FROM EMPLOYEES E1
WHERE NOT EXISTS
(
SELECT *
FROM EMPLOYEES E2
WHERE E2.DEPARTMENT_ID = E1.DEPARTMENT_ID AND
E2.SALARY > E1.SALARY
) ;


--sub query in from clause 

--show the last name and salary of each employee along with the
--minimum salary and maximum salary of his/her department
SELECT E1.LAST_NAME, E1.SALARY, MIN(E2.SALARY), MAX(E2.SALARY)
FROM EMPLOYEES E1 JOIN EMPLOYEES E2
ON (E1.DEPARTMENT_ID = E2.DEPARTMENT_ID)
GROUP BY E1.DEPARTMENT_ID, E1.LAST_NAME, E1.SALARY
ORDER BY E1.SALARY ;

--same using nested query
SELECT E.LAST_NAME, E.SALARY, D.MINSAL, D.MAXSAL
FROM EMPLOYEES E,(
SELECT DEPARTMENT_ID AS DEPT, MIN(SALARY) MINSAL, MAX(SALARY) MAXSAL
FROM EMPLOYEES
GROUP BY DEPARTMENT_ID
) D
WHERE (E.DEPARTMENT_ID = D.DEPT)
ORDER BY E.SALARY ;




------------------------- CHAPTER 07 (SET OPTN)------------------------
SELECT EMPLOYEE_ID,JOB_ID
FROM EMPLOYEES
UNION(
    SELECT EMPLOYEE_ID, JOB_ID
    FROM JOB_HISTORY
);


SELECT EMPLOYEE_ID, JOB_ID
FROM EMPLOYEES
UNION ALL                   -- NOMRALLY set opn duplicate shoraye dey
(
SELECT EMPLOYEE_ID, JOB_ID
FROM JOB_HISTORY
) ;


SELECT EMPLOYEE_ID, JOB_ID
FROM EMPLOYEES
INTERSECT
(
SELECT EMPLOYEE_ID, JOB_ID
FROM JOB_HISTORY
) ;


--The following query finds the employees who have not changed their jobs even once
SELECT EMPLOYEE_ID, JOB_ID
FROM EMPLOYEES
MINUS
(
SELECT EMPLOYEE_ID, JOB_ID  -- minus maane holo second tay thakle baaad, ekhane job history te karor naam thaka mane age onno job e chilo
FROM JOB_HISTORY
) ;


SELECT LAST_NAME, JOB_ID, SALARY
FROM EMPLOYEES
UNION ALL
(
SELECT LAST_NAME, JOB_ID, SALARY
FROM EMPLOYEES2
) ;

---practise 7.1

-- a. Find EMPLOYEE_ID of those employees who are not managers. Use minus operator to
-- perform this.
-- b. Find last names of those employees who are not managers. Use minus operator to perform this.
-- c. Find the LOCATION_ID of those locations having no department

select EMPLOYEE_ID from EMPLOYEES
MINUS(
    SELECT MANAGER_ID from EMPLOYEES 
);

select last_name from EMPLOYEES
where EMPLOYEE_ID IN (select EMPLOYEE_ID from EMPLOYEES
MINUS(
    SELECT MANAGER_ID from EMPLOYEES 
));

SELECT LOCATION_ID from LOCATIONS
MINUS(
    select LOCATION_ID from DEPARTMENTS
);



