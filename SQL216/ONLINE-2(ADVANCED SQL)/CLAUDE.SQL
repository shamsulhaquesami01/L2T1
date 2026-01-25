-- Set 1: Complex Aggregations & Subqueries
-- Q1. List employees whose jobs have maximum salaries higher than the overall company average maximum salary, 
-- for jobs located in departments in Seattle and London.
-- Q2. Find departments that both have more than 3 managers 
-- AND have average salaries greater than the overall average salary across all departments.
-- Q3. Write a SQL query for departments that have employees, with no location changes in job history, 
-- and average salary > company average. Show department_name, avg_salary, location, 
-- and label 'Premium Department' if avg_salary > 1.5 times company average, else 'Above Company Avg'.


--Q1
SELECT E.EMPLOYEE_ID FROM EMPLOYEES E
JOIN DEPARTMENTS D ON D.DEPARTMENT_ID=E.DEPARTMENT_ID
JOIN LOCATIONS L ON L.LOCATION_ID=D.LOCATION_ID
WHERE
(SELECT J.MAX_salary FROM JOBS J WHERE J.JOB_ID=E.JOB_ID) > 
(SELECT AVG(J2.MAX_salary) FROM JOBS J2) AND
L.CITY = 'Seattle' OR L.CITY ='London';

--Q2

SELECT * FROM DEPARTMENTS D
SELECT * FROM EMPLOYEES E ORDER BY E.DEPARTMENT_ID;