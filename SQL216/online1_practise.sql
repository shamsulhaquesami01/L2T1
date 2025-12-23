

-- ***** CHAPTER 2 **********

SELECT *FROM EMPLOYEES;

SELECT employee_id, last_name, salary*12 yearly --ek word hole normally
from EMPLOYEES;

SELECT employee_id, last_name, salary*12 "yearly salary" --space thakle double ""
from EMPLOYEES;

SELECT LAST_NAME, COMMISSION_PCT
FROM EMPLOYEES ;

SELECT LAST_NAME, (SALARY+SALARY*COMMISSION_PCT) SALCOMM
FROM EMPLOYEES ;

SELECT (FIRST_NAME ||' ' || SALARY) FULLNAME, SALARY --concat two columns
FROM EMPLOYEES ;

SELECT ('NAME is: ' || FIRST_NAME || ' ' || LAST_NAME) FULLNAME, SALARY  --concat korar shomoy text add korle single '', double "" & nothing only column name er jonno
FROM EMPLOYEES ;

SELECT ('NAME is: ' ||123|| FIRST_NAME || ' ' || LAST_NAME) FULLNAME, SALARY  
FROM EMPLOYEES ;  --number add korte '' laage na, chaile '' er moddhe text hishebeo add deya jabe

SELECT DISTINCT DEPARTMENT_ID, JOB_ID
FROM EMPLOYEES ;  --ekhane distinct element ashbe but jehetu duita bolse so erokom hote pare jey dept_id distinct but job na taholeo oi row gula ashbe abar vice verca
--ekhane sa_rep duibar karon 80 & null

DESCRIBE EMPLOYEES ;

/*a. Write an SQL query to retrieve all country names.
b. Write an SQL query to retrieve all job titles.
c. Write an SQL query to retrieve all MANAGER_IDs.
d. Write an SQL query to retrieve all city names. Remove duplicate outputs.
e. Write an SQL query to retrieve LOCATION_ID, ADDRESS from LOCATIONS table. The
ADDRESS should print each location in the following format: STREET_ADDRESS, CITY,
STATE_PROVINCE, POSTAL_CODE
*/

SELECT country_name from COUNTRIES;
SELECT MANAGER_ID from EMPLOYEES;
SELECT JOB_TITLE from jobs;
SELECT DISTINCT city from LOCATIONS;
SELECT LOCATION_ID, (STREET_ADDRESS ||','||  CITY||','||   STATE_PROVINCE ||','|| POSTAL_CODE) ADDRESS
from LOCATIONS;
-------------------------------------------------------------------------------------------------------------------------------------------------

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME = 'Whalen' ;

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE HIRE_DATE = '01-JAN-1995' ;

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE COMMISSION_PCT IS NULL ; -- = NUll dile hobe na; null = null is false

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE HIRE_DATE BETWEEN '01-JAN-2003' AND '31-DEC-2005' ; --between is inclusive of the conditions

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME IN ('Ernst', 'Austin', 'Pataballa', 'Lorentz') ;  --match with set of values

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE DEPARTMENT_ID <> 80 AND
(SALARY > 5000 OR COMMISSION_PCT IS NOT NULL) ;


--use of like--

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE 'S%' ; --case sensitive; pore % mane pore joto iccha(0- n) ta letter thakte parbe (starts with S)


SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE '%s' ; --age % mane age ja iccha thakte parbe (ends with s)

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE '%s%' ;  --age pore % mane naame s thaklei hobe jekono jaygay (contanins s)

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE 's' ;  --kono % nai mane last_name=s naamtai s ; (name =s)

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE 'a_' ; --contains exactly two characters where the first character must be ‘a’

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE '_ _ b' ; --contains exactly three characters in which the last one must be ‘b’

SELECT LAST_NAME, SALARY
FROM EMPLOYEES
WHERE LAST_NAME LIKE '_%' ;  --contains at least one character, i.e.,last name cannot be empty text



SELECT LAST_NAME, SALARY, COMMISSION_PCT
FROM EMPLOYEES
WHERE COMMISSION_PCT < 0.20 OR COMMISSION_PCT IS NULL ;  -- null mane 0 dhorle alada kore include korte hobe
-- normally null er sathe jeokono comparision false return korbe mane oi row ashbe na


/* PRACTISE 2.2
a. Select names of all employees who have joined before January 01, 2004.
b. Select all locations in the following countries: Canada, Germany, United Kingdom.
c. Select first names of all employees who do not get any commission.
d. Select first names of employees whose last name starts with an 'a'.
e. Select first names of employees whose last name starts with an 's' and ends with an 'n'.
f. Select all department names whose MANAGER_ID is 100.
g. Select all names of employees whose job type is 'AD_PRES' and whose salary is at least 23000.
h. Select names of all employees whose last name do not contain the character 's'.
i. Select names and COMMISSION_PCT of all employees whose commission is at most 0.30.
j. Select names of all employees who have joined after January 01, 1998.
k. Select names of all employees who have joined in the year 1998.
*/
SELECT (FIRST_NAME ||' '|| last_name) NAME 
    from EMPLOYEES
    WHERE HIRE_DATE < '01-JAN-2004' ;

/*Select * from locatins 
    where COUNTRY_ID IN
        (SELECT COUNTRY_ID    problem
        from COUNTRIES
        where country_name IN ('Canada', 'Germany', 'United Kingdom'));*/ 

SELECT first_name from EMPLOYEES 
    WHERE COMMISSION_PCT IS NOT null;

SELECT first_name from EMPLOYEES 
    WHERE LAST_NAME LIKE 'A%';

SELECT first_name from EMPLOYEES 
    WHERE LAST_NAME LIKE 'S%n';

SELECT department_name from DEPARTMENTS
    WHERE MANAGER_ID =100;

SELECT (FIRST_NAME || ' ' || LAST_NAME) NAME from EMPLOYEES
    where LAST_NAME NOT LIKE '%s%';

SELECT (FIRST_NAME ||' '|| last_name) NAME 
    from EMPLOYEES
    WHERE HIRE_DATE > '01-JAN-2007' ;


---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--sorting

SELECT JOB_TITLE, (MAX_SALARY - MIN_SALARY) DIFF_SALARY
FROM JOBS
ORDER BY DIFF_SALARY DESC , JOB_TITLE DESC;  --derived coloumn er sorting o hobe, comma diye multiple ektar por ekta


/*
Practice 2.3
a. Select names, salary, and commissions of all employees of job type 'AD_PRES'. Sort the result
in ascending order of commission and then descending order of salary.
b. Retrieve all country names in lexicographical ascending order.
*/

SELECT (FIRST_NAME || ' ' || LAST_NAME) NAME, SALARY, COMMISSION_PCT from EMPLOYEES
    WHERE JOB_ID='SA_REP'
    ORDER BY COMMISSION_PCT ASC, SALARY DESC;

SELECT * from employees;



---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- ***** CHAPTER 3 **********

-------------------- DATE Functions----------------------------

select sysdate from dual; --dual empty table for checking functions; sysdate current date and time

SELECT round((SYSDATE - HIRE_DATE)/7) "WEEKS EMPLOYED" FROM EMPLOYEES
    WHERE DEPARTMENT_ID = 80 ;

SELECT LAST_NAME, trunc(MONTHS_BETWEEN(SYSDATE, HIRE_DATE)) MON_EMPLOYED
FROM EMPLOYEES ;

SELECT ADD_MONTHS(SYSDATE, 5) from dual;

SELECT round(sysdate, 'month') from dual;
SELECT round(sysdate, 'year') from dual;
SELECT round(TO_DATE('01-JUN-1996'), 'year') from dual;

SELECT trunc(sysdate, 'month') from dual;
SELECT trunc(sysdate, 'year') from dual;
SELECT trunc(TO_DATE('31-DEC-1996'), 'year') from dual;

/*
Practice 3.3
a. For all employees, find the number of years employed. Print first names and number of years
employed for each employee.
b. Suppose you need to find the number of days each employee worked during the first month of
his joining. Write an SQL query to find this information for all employees.
*/

SELECT first_name, round((sysdate- HIRE_DATE)/365) "YEARS employed"
    from EMPLOYEES;

SELECT first_name, (last_day(HIRE_DATE)-HIRE_DATE +1 ) "1st Month Workday"  
    from EMPLOYEES; --Last_day : function that automatically finds the last day of the month for any given date (handling leap years in February automatically)



-------------------- NUMBER Functions----------------------------


SELECT LAST_NAME, ROUND(SALARY/1000, 2) "SALARY (IN THOUSANDS)"
FROM EMPLOYEES
WHERE INSTR(JOB_ID, 'CLERK') > 0 ;

SELECT LAST_NAME, ROUND( (SYSDATE - HIRE_DATE)/7, 4) WEEKS_EMPLOYED
FROM EMPLOYEES
WHERE DEPARTMENT_ID = 80 ;

SELECT LAST_NAME, TRUNC(SALARY/1000, 0) || ' thousands ' ||
TRUNC( MOD(SALARY,1000)/100, 0) || ' hundreds ' || MOD(SALARY,100) || ' taka only'
FROM EMPLOYEES ;

/*
Practice 3.2
a. Print employee last name and number of days employed. Print the second information rounded
up to 2 decimal places.
b. Print employee last name and number of years employed. Print the second information
truncated up to 3 decimal place.
*/

SELECT LAST_NAME, ROUND( (SYSDATE - HIRE_DATE), 2) DAYS_EMPLOYED
    FROM EMPLOYEES;

SELECT LAST_NAME, ROUND( (SYSDATE - HIRE_DATE)/365, 3) YEARSS_EMPLOYED
    FROM EMPLOYEES;

-------------------- Character Functions----------------------------

SELECT (INITCAP(FIRST_NAME) || LOWER(LAST_NAME)) NAME, UPPER(JOB_ID) JOB
    FROM EMPLOYEES ;  -- initcap mane only first letter boro haater bakishob choto

SELECT *FROM DEPARTMENTS
    WHERE UPPER(DEPARTMENT_NAME) LIKE '%SALE%' ; -- to match the word irrespective of case, we convert everything to lower than match

SELECT *FROM DEPARTMENTS
    WHERE LOWER(DEPARTMENT_NAME) LIKE '%sale%' ;

SELECT CONCAT(FIRST_NAME, LAST_NAME) NAME, JOB_ID  -- concat funtion ar || same kaaj
    FROM EMPLOYEES
    WHERE INSTR( UPPER(JOB_ID), 'CLERK') > 0 ;   -- etao chaile LIKE %CLERK% diye kora jaabe

SELECT ( SUBSTR(FIRST_NAME, 1, 1) || '.' || SUBSTR(LAST_NAME, 1, 1) || '.' ) ABBR
    FROM EMPLOYEES ;

SELECT INITCAP( TRIM(LAST_NAME) ) NAME, LPAD(SALARY, 10, 0) 
    FROM EMPLOYEES ;


/*
Practice 3.1
a. Print the first three characters and last three characters of all country names. Print in capital
letters.
b. Print all employee full names (first name followed by a space then followed by last name). All
names should be printed in width of 60 characters and left padded with '*' symbol for names
less than 60 characters.
c. Print all job titles that contain the text 'manager'.
*/



-------------------------------Chapter 4: Aggregate Functions

SELECT DEPARTMENT_ID, SUM(SALARY) "TOTAL SALARY OF DEPT"
    FROM EMPLOYEES
        GROUP BY DEPARTMENT_ID ;

SELECT JOB_ID, MAX(SALARY), MIN(SALARY), AVG(SALARY)
    FROM EMPLOYEES
        GROUP BY JOB_ID ;
 
SELECT JOB_ID, JOB_TITLE, COUNT(*) TOTAL  --should be either in group by or in aggregation
    FROM JOBS
        GROUP BY JOB_ID; --error

SELECT DEPARTMENT_ID, COUNT(LAST_NAME)
    FROM EMPLOYEES
        GROUP BY DEPARTMENT_ID ;

SELECT MAX(SALARY), MIN(SALARY), AVG(SALARY)
    FROM EMPLOYEES ;
    
SELECT COUNT(*)
    FROM EMPLOYEES ;

SELECT JOB_ID, MAX(SALARY), MIN(SALARY)
    FROM EMPLOYEES
        WHERE DEPARTMENT_ID = 80
        GROUP BY JOB_ID ;

SELECT JOB_ID, MAX(SALARY), MIN(SALARY)
    FROM EMPLOYEES
        WHERE DEPARTMENT_ID = 80
        GROUP BY JOB_ID
        ORDER BY JOB_ID ASC ;   


/*
a. For all managers, find the number of employees he/she manages. Print the MANAGER_ID
and total number of such employees.
b. For all departments, find the number of employees who get more than 30k salary. Print the
DEPARTMENT_ID and total number of such employees.
c. Find the minimum, maximum, and average salary of all departments except
DEPARTMENT_ID 80. Print DEPARTMENT_ID, minimum, maximum, and average salary.
Sort the results in descending order of average salary first, then maximum salary, then
minimum salary. Use column alias to rename column names in output for better display.

*/

select manager_id , count(*) "Numebr of employees"
            from EMPLOYEES
            group by manager_id;

select department_id , count(*) "Number of employees"
        from EMPLOYEES
       where SALARY >10000
        group by department_id;

select department_id, round(avg(salary)) average, max(salary) maximum, min(salary) minimum
        from employees
        where DEPARTMENT_ID != 80
        group by DEPARTMENT_ID
        order BY avg(salary), max(salary), min(salary) DESC;


------------------------------------------------------------------------------------------------------------------------------------------------

SELECT JOB_ID, MAX(DISTINCT SALARY), MIN(DISTINCT SALARY), --distinct dile duplicate row ekbar nibe
SUM(DISTINCT SALARY), COUNT(DISTINCT DEPARTMENT_ID) --count & sum change hobe
FROM EMPLOYEES 
WHERE DEPARTMENT_ID = 80
GROUP BY JOB_ID
ORDER BY JOB_ID ASC ;


SELECT JOB_ID, MAX(SALARY), MIN(SALARY) -- jei group gular max 5k theke beshi
FROM EMPLOYEES
GROUP BY JOB_ID
HAVING MAX(SALARY) > 5000 ;  -- group na kora porjonto jana possible na


SELECT JOB_ID, AVG(SALARY)
FROM EMPLOYEES
GROUP BY JOB_ID
HAVING AVG(SALARY) <= 5000 --same goes for avg
ORDER BY AVG(SALARY) DESC ;


----------------------------------------------------4.3----------------------------------------------------------------------

SELECT TO_CHAR(HIRE_DATE, 'YYYY') YEAR, COUNT(*) TOTAL
FROM EMPLOYEES
GROUP BY TO_CHAR(HIRE_DATE, 'YYYY')
ORDER BY YEAR ASC ;

SELECT SUBSTR(FIRST_NAME, 1, 1) FC, COUNT(*) TOTAL
FROM EMPLOYEES
GROUP BY SUBSTR(FIRST_NAME, 1, 1)
ORDER BY FC ASC ;

SELECT DEPARTMENT_ID, JOB_ID, COUNT(*) TOTAL
FROM EMPLOYEES
GROUP BY DEPARTMENT_ID, JOB_ID ;

SELECT JOB_ID|| '  HI', JOB_TITLE, COUNT(*) TOTAL
FROM JOBS
GROUP BY JOB_ID, JOB_TITLE ;

------exercise
/*
a. Find number of employees in each salary group. Salary groups are considered as follows.
Group 1: 0k to <5K, 5k to <10k, 10k to <15k, and so on.
b. Find the number of employees that were hired in each year in each job type. Print year, job id,
and total employees hired.
*/

SELECT 
    (TRUNC(SALARY / 5000) * 5) || 'k to <' || ((TRUNC(SALARY / 5000) + 1) * 5) || 'k' AS SALARY_GROUP,
    COUNT(*) AS TOTAL_EMPLOYEES
FROM 
    EMPLOYEES
GROUP BY 
    TRUNC(SALARY / 5000)
ORDER BY 
    TRUNC(SALARY / 5000) ASC;

--ALTERNATE
SELECT 
    TRUNC(SALARY/ 5000)  AS SALARY_GROUP,
    COUNT(*) AS TOTAL_EMPLOYEES
FROM 
    EMPLOYEES
GROUP BY 
    TRUNC(SALARY / 5000)
ORDER BY 
    TRUNC(SALARY / 5000) ASC;


SELECT TO_CHAR(HIRE_DATE, 'DD') year, job_id, count(*) total
        from EMPLOYEES
        group by TO_CHAR(HIRE_DATE, 'DD'),job_id
        order by TO_CHAR(HIRE_DATE, 'DD') ASC;
