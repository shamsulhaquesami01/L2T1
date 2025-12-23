/*
Find the employee id, full name (first name <space> last name), department id,
total monthly salary with commission percentage of the employees whose first
name starts with ‘D’ and the fourth character of the last name is ‘n’ and works in
departments with ID between 20 and 70. (Assume zero commission percentage
for the employees with null commission percentage)
*/
SELECT employee_id, first_name ||' '|| last_name full_name, department_id, salary+(salary*nvl(commission_PCT,0)) "SALARY"
        FROM employees
        WHERE FIRST_NAME LIKE 'D%' AND LAST_NAME LIKE '___n%' AND (DEPARTMENT_ID BETWEEN 20 and 70);

/*
2. Display the job IDs of jobs that were held by at least two employees for a duration
of at most 1500 days.
3. Show the department_id, minimum, maximum, and average salary of all
departments except depeartment_id 50. Sort the list in descending order of the
difference between maximum salary and minimum salary of each department
and then sort by average salary in ascending order. (The average salary value is
rounded to four decimal digits)
4. For each manager who manages less than 10 employees, show the manager id,
number of employees and the average salary of the employees managed by
him/her. Show the outputs with an average salary less than 4000. Print the output
in the ascending order of number of employees managed and if there is a tie then
print those in descending order of average salary. Make sure any sort of null value
is not printed.
5. Find the last name of those employees whose last name contains odd number of
consonants.
*/



--2


select job_id 
  from EMPLOYEES 
  where (sysdate-HIRE_DATE) <=1500
  group by JOB_id
  HAVING count(*) >=2;

SELECT JOB_ID
FROM JOB_HISTORY
WHERE (END_DATE - START_DATE) <= 1500
GROUP BY JOB_ID
HAVING COUNT(*) >= 2;

--3

select DEPARTMENT_ID, min(salary) minimum, max(salary) maximum, round(avg(salary),4) average
        from EMPLOYEES
        where DEPARTMENT_ID <> 50
        group by DEPARTMENT_ID
        order by maximum-minimum DESC, average ASC;

--4

select manager_id,count(*) as "number of employees", avg(salary) average
        from EMPLOYEES
        where MANAGER_ID is not NULL
        GROUP by MANAGER_ID
        having count(*) <10 and avg(salary) < 4000
        order by count(*) asc, average desc;

--5

select last_name from EMPLOYEES
        where mod(length(last_name) - length(translate(lower(last_name), 'dAEIOUaeiou', 'd')),2) <>0;