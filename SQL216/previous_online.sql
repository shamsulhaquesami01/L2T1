/*
Find the employee id, full name (first name <space> last name), department id,
total monthly salary with commission percentage of the employees whose first
name starts with ‘D’ and the fourth character of the last name is ‘n’ and works in
departments with ID between 20 and 70. (Assume zero commission percentage
for the employees with null commission percentage)
*/
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



SELECT employee_id, first_name ||' '|| last_name full_name, department_id, salary+(salary*nvl(commission_PCT,0)) "SALARY"
        FROM employees
        WHERE FIRST_NAME LIKE 'D%' AND LAST_NAME LIKE '___n%' AND (DEPARTMENT_ID BETWEEN 20 and 70);


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

------------------- ------------------------- ------------ SEC(C) ---------------------------- ---------------------------- ---------------------------- 

/*
1. For each country, show country_id, number of cities and number of provinces. Sort
the output in ascending order of country_id. Discard those countries which have no
provinces and less than two cities.
2. Display department IDs in which at most 1 employee had worked for at most 600 days.
3. Show the department_id, minimum hire_date, maximum hire_date, and average
salary of all departments. Sort the list in ascending order of the difference between
maximum and minimum hire_date of each department and then sort by average
salary in descending order. Make sure no null values are printed.
4. For each employee, display their full name (full name includes first name, a space
and last name), current annual salary, current monthly salary, commission
percentage, and their next year’s increased monthly salary. Consider that employees
will get a year-end increment according to the formula (increment = annual salary *
commission percentage / 100). Employees who do not have any defined commission
percentage will get a 0.05% pay deduction (based on their annual salary) at the end
of the year.
5. For each manager, show the manager id, number of employees under his
management who joined in March, September or December and average salary of
those employees. Show the outputs where the average salary is integer. Print the
output in the ascending order of number of employees managed and if there is a tie
then print those in descending order of average salary. Make sure any sort of null
value is not printed.
*/

select * from LOCATIONS;
select country_id, count(city), count(STATE_PROVINCE)
        from LOCATIONS
        group by country_id
        having count(city) >=2 AND count(STATE_PROVINCE) <> 0
        order by COUNTRY_ID asc;

SELECT DEPARTMENT_ID
FROM (
    SELECT DEPARTMENT_ID, COUNT(*) as SHORT_TERM_COUNT
    FROM JOB_HISTORY
    WHERE (END_DATE - START_DATE) <= 600
    GROUP BY DEPARTMENT_ID
) 
WHERE SHORT_TERM_COUNT <= 1;

select department_id, min(hire_date) minimum, max(hire_date) maximum, round(avg(salary),2) average
        FROM Employees
        where DEPARTMENT_ID is not null
        group by department_id
        order by maximum-minimum ASC, average DESC;


select first_name||' '||last_name as full_name, salary*12 as annual, salary as monthly,
            commission_PCT, (salary*12)+((salary*12)* nvl(commission_PCT,0.05)) as next
            from employees;

select manager_id, count(*) as " number of employees", avg(salary) as average
            from employees
            where manager_id is not null and 
            TO_CHAR(hire_date, 'MM') in ('03', '09','12')
            group by MANAGER_ID
            having mod( avg(salary),1)=0
            order by count(*) asc, avg(salary) desc;


-- 1. Count the number of employees hired during each quarter of the year. [Hint: You
-- can convert a number (e.g., 1) into its corresponding month ‘Jan’ by using the
-- function to_char (to_date (1,‘MM’), ‘Mon’)]
-- 2. Display the full name (full name includes first name, a space and last name) of
-- the employees formatted as right justified in the column, in such a way that a total
-- 20 characters are shown including the name. Order the result in ascending order
-- of the length of their full names.
-- 3. Display the country id and address for each of the locations. Address should be
-- generated in the format (street_address, city, state_province - postal_code). Your
-- address should be displayed only when all the required fields are available. Order
-- the result in lexicographic order of the country id. In case of any tie, break it by the
-- reverse lexicographic order of the postal code.
-- 4. For each department and each job, show the department id, job id, the first hiring
-- date, the last hiring date and average salary. Show the outputs with an average
-- salary more than 8000. Make sure any sort of null value is not printed and the
-- average salary value is rounded to two decimal digits. Order the result by
-- department id.
-- 5. Show the full name (full name includes first name, a space and last name) and
-- hire date of all employees whose first name starts with a consonant and the last
-- name does not contain the letter B/b, and who have joined in November. Show
-- the hire date along with the full name of the employees. Show the results in the
-- lexicographical order of full name.


select CEIL(to_char (hire_date, 'MM')/3) ,count(*) 
        from EMPLOYEES
        group by CEIL(to_char (hire_date, 'MM')/3) 
        order by CEIL(to_char (hire_date, 'MM')/3) asc;

select lpad(first_name||' '||last_name,20,' ') as "full_name"
        from EMPLOYEES
        order by length(first_name||' '||last_name) asc;


select country_id, street_address||','|| city||','||state_province||'-'||postal_code as address
        from LOCATIONS
        where COUNTRY_ID is not null and STREET_ADDRESS is not null and  city is not null
         and  STATE_PROVINCE is not null and postal_code is not NULL
         order by country_id ASC , postal_code asc;

select department_id, job_id, min(hire_date), max(hire_date), round(avg(salary),2) average
                    from employees
                    where DEPARTMENT_ID is not null and JOB_ID is not null
                    group by DEPARTMENT_ID, JOB_ID
                    order by DEPARTMENT_ID ASC;


select first_name||' '||last_name full_name, hire_date from EMPLOYEES
        where substr(lower(first_name),1,1) not in ('a','e','i','o','u') AND
              lower(last_name) not like '%b%' AND to_char(hire_date, 'mm')='11'
        order by full_name asc;



--hi hello how are you
--email vul korsi re

//hayre