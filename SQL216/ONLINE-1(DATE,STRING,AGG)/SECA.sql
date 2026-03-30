-- Group employees by their hiring month, using the three-letter format (JAN, FEB, …, 
-- DEC). For each month, display the number of employees whose last name contains 
-- the letter “a” (case-insensitive). Sort the results chronologically from JAN to DEC. 
select to_char(hire_date,'MON') as month ,count(*) as "Number of Employees" from EMPLOYEES
    where lower(last_name) like '%a%'
        GROUP by to_char(hire_date,'MON')
        order by to_date(month,'MM') asc;

-- Show the employee ID and last name of employees whose last name contains at 
-- least one pair of identical consecutive characters (e.g., AA, bB, Cc, dd, etc.). Sort by 
-- last name

select employee_id,last_name from EMPLOYEES 
    WHERE 
       lower(LAST_NAME) LIKE '%aa%' OR lower(LAST_NAME) like '%bb%' OR lower(LAST_NAME) like '%cc%' OR lower(LAST_NAME) like '%dd%'
    OR lower(LAST_NAME) like '%ee%' OR lower(LAST_NAME) like '%ff%' OR lower(LAST_NAME) like '%gg%' OR lower(LAST_NAME) like '%hh%'
    OR lower(LAST_NAME) like '%ii%' OR lower(LAST_NAME) like '%jj%' OR lower(LAST_NAME) like '%kk%' OR lower(LAST_NAME) like '%ll%'
    OR lower(LAST_NAME) like '%mm%' OR lower(LAST_NAME) like '%nn%' OR lower(LAST_NAME) like '%oo%' OR lower(LAST_NAME) like '%pp%'
    OR lower(LAST_NAME) like '%qq%' OR lower(LAST_NAME) like '%rr%' OR lower(LAST_NAME) like '%ss%' OR lower(LAST_NAME) like '%tt%'
    OR lower(LAST_NAME) like '%uu%' OR lower(LAST_NAME) like '%vv%' OR lower(LAST_NAME) like '%ww%' OR lower(LAST_NAME) like '%xx%'
    OR lower(LAST_NAME) like '%yy%' OR lower(LAST_NAME) like '%zz%'
    order by last_name ;





 
-- Create a table named PERFORMANCE_REVIEW with columns: REVIEW_ID, 
-- EMPLOYEE_ID, REVIEW_YEAR, RATING, REMARKS. 
 
-- Ensure that: 
-- ● Each rating is linked to an employee 
-- ● Rating must be between 1 and 5 
-- ● Remarks are optional 
 
-- Insert one record into the table

CREATE TABLE PERFORMANCE_REVIEW(
    REVIEW_ID NUMBER(20),
    EMPLOYEE_ID NUMBER(6),
    REVIEW_YEAR NUMBER(5) not null,
    RATING NUMBER(5) not null,
    REMARKS VARCHAR2(200),
    CONSTRAINT pk PRIMARY KEY(REVIEW_ID),
    CONSTRAINT fik FOREIGN KEY(EMPLOYEE_ID) REFERENCES EMPLOYEES(EMPLOYEE_ID),
    CONSTRAINT range_ck CHECK(RATING IN (1,2,3,4,5))
);
INSERT INTO PERFORMANCE_REVIEW VALUES(1234,102,2025,3,'nothing');
INSERT INTO PERFORMANCE_REVIEW(REVIEW_ID,EMPLOYEE_ID,REVIEW_YEAR,RATING) VALUES(456,103,2024,2);

select * from PERFORMANCE_REVIEW;

-- Find the employee ID, full name (first name <space> last name), and a generated 
-- "User ID" based on the following conditions: 
-- ● The User ID should consist of the first 3 letters of the first name combined with 
-- the last 3 digits of their phone number. 
-- ● Filter for employees whose phone number contains at least 10 characters and 
-- works in department 50 or 80. 
-- ● Sort the results by the length of their first name in descending order. 
-- You should write a single SQL query to achieve the desired result. No need to 
-- write separate SQL queries.
SELECT employee_id, first_name||' '||last_name as fullname, substr(first_name,1,3)||substr(phone_number,-3,3) as "User ID"
    FROM EMPLOYEES
    WHERE LENGTH(PHONE_NUMBER)>=10 AND (DEPARTMENT_ID = 80 OR DEPARTMENT_ID =50)
    ORDER BY LENGTH(FIRST_NAME) DESC;


-- Consider your given HR schema. Display the Job ID and the "Salary Gap" (difference 
-- between Maximum and Minimum salary) for jobs where the gap is greater than 3000. 
-- Exclude any jobs where the average salary is less than 6000. Then finally, sort the 
-- result by the Salary Gap in descending order. 


SELECT JOB_ID, MAX(Salary)-MIN(Salary) AS "Salary Gap" 
    FROM EMPLOYEES
    GROUP BY JOB_ID
    HAVING MAX(Salary)-MIN(Salary) > 3000 AND AVG(SALARY) >=6000
    ORDER BY MAX(Salary)-MIN(Salary) DESC;