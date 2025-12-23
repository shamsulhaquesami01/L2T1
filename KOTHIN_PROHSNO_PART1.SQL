-- Find employees whose first name has exactly 5 characters,
-- starts with a vowel, ends with a consonant,
-- and whose last name contains at least 2 vowels but NO consecutive vowels.
-- Display full name and email. 
-- Sort by the number of vowels in their full name (descending), 
-- then alphabetically.

select first_name||' '||last_name fullname, email from employees
    where first_name like '_____' AND SUBSTR(lower(FIRST_NAME),5,1) not IN ('a','e','i','o','u') AND
    SUBSTR(lower(FIRST_NAME),1,1) IN ('a','e','i','o','u') AND
    length(last_name)-length(translate(lower(last_name), 'daeiou', 'd'))>=2
    order by length(FULLNAME)-length(translate(lower(FULLNAME), 'daeiou', 'd')) desc, FULLNAME asc;

-- Display the employee_id and last_name of employees whose last name reads the same forwards and backwards 
-- (case-insensitive). Additionally, show the length of their last name.
--  Exclude names with less than 4 characters.

select employee_id , last_name , length(last_name) from EMPLOYEES
   where lower(LAST_NAME)= reverse(lower(LAST_NAME)) AND length(last_name) >=4;

-- For each employee, extract the middle character(s) of their full name (first_name + last_name without space).
--  If the total length is odd, show 1 character; 
--  if even, show 2 middle characters. Display employee_id, full_name, and middle_char(s)
select employee_id,first_name||last_name fullname, 
 case
    when mod(length(first_name||last_name)/2,2) <> 0 
            then substr(first_name||last_name, (length(first_name||last_name)+1)/2,1)
    else substr(first_name||last_name, length(first_name||last_name)/2,2)
 end
    as middle_name

from EMPLOYEES;

-- Show all employees with their full_name, salary, commission_pct, 
-- and a calculated field 'effective_monthly_income'. The formula is:

-- If commission_pct exists: (salary + salary * commission_pct/100) / 12
-- If commission_pct is NULL but salary > 5000: salary / 12 * 1.05
-- If both commission_pct is NULL and salary <= 5000: salary / 12 * 0.95

select employee_id,first_name||last_name fullname, salary, commission_PCT,
       (salary + salary * nvl(commission_pct,
        CASE
        when salary > 5000
            then 1.05
        else 0.95
        end
       )/100) 
       as effective_income
            from EMPLOYEES;


-- Find all employees hired on February 29th (leap year day). 
-- Show their employee_id, full name, hire_date, 
-- and the next leap year date after their hire date. Sort by hire_date.

select employee_id,employee_id,first_name||last_name fullname,hire_date,
    to_char(HIRE_DATE+365*4 ,'YYYY')as next
        from EMPLOYEES
            where to_char(HIRE_DATE,'MM') = '02' AND to_char(HIRE_DATE,'DD') = '29'
                order by hire_date;


-- Calculate the number of WORKING DAYS (excluding weekends - Saturday and Sunday) 
-- each employee has worked until today. Show employee_id, hire_date, and working_days. 
-- Include only employees hired after January 1, 2020. Sort by working_days descending
 
SELECT employee_id,
       hire_date,
       -- Count working days between hire_date and today
       (TRUNC(SYSDATE) - hire_date + 1)
       - 2 * (TRUNC(SYSDATE, 'IW') - TRUNC(hire_date, 'IW')) / 7
       - CASE WHEN TO_CHAR(hire_date, 'DY', 'NLS_DATE_LANGUAGE=ENGLISH') IN ('SAT','SUN')
              THEN 1 ELSE 0 END
       - CASE WHEN TO_CHAR(SYSDATE, 'DY', 'NLS_DATE_LANGUAGE=ENGLISH') = 'SAT'
              THEN 1
              WHEN TO_CHAR(SYSDATE, 'DY', 'NLS_DATE_LANGUAGE=ENGLISH') = 'SUN'
              THEN 1 ELSE 0 END
       AS working_days
FROM   employees
WHERE  hire_date > DATE '2020-01-01'
ORDER  BY working_days DESC;

-- Find employees hired in the LAST WEEK of any quarter (Q1, Q2, Q3, Q4).
--  Show employee_id, hire_date, quarter, and days_until_next_quarter.
--   Sort by quarter, then hire_date.

select employee_id, to_char(hire_date, 'Q') as quarter,hire_date,
    CASE
        when
            to_char(hire_date, 'MM') in ('03','12')
                then 31 - to_char(hire_date,'DD')
        else
            30 - to_char(hire_date,'DD')
    end

     from EMPLOYEES
        where to_char(hire_date, 'MM') in ('03','06','09','12') AND
              to_char(hire_date, 'DD') in ('24','25','26','27','28','29','30','31');