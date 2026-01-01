-- Show all the employees full name and salary who are not from department 80 and have a
-- salary greater than 6000 or a non-NULL commission percentage in descending order of their
-- commission and then ascending order of their last name.

SELECT employee_id, first_name ||' '|| last_name full_name,salary from EMPLOYEES
    where department_id <> 80 AND (salary >= 5000 OR commission_PCT is not null)
    order by commission_PCT desc, last_name asc;


-- The conventional method for creating an email is to combine the initial letter from the First
-- Name field (If it has space then the first character after space too) with the entire Last Name
-- field (without space), both in uppercase, to form the EMAIL field. Nevertheless, some
-- employees might not have their entire last name included in the email. Find employees
-- whose emails don't include their entire last names. Example output.

select first_name, last_name, email
from employees
where upper(email) not like '%'||upper(replace(last_name,' ',''))||'%';

-- The company is thinking of updating the policy. They have decided to set the minimum
-- salary for any employee to be 8100. How much extra money will they need to spend to
-- accommodate this new policy? Find this amount by writing an sql query in the EMPLOYEES
-- table.

select sum(8100-salary) from EMPLOYEES
        where SALARY<8100;

-- Write a single query to find the total number of employees who are charged with
-- commissions and who are not. The Indicator here is boolean that shows 0 for not being
-- charged and 1 for being charged. Your output should exactly match the following:

select 
(case 
  when COMMISSION_PCT is  null then 0||'(No)'
  else 1||'(YES)'
  end
) as "IS_COMMISSION_CHARGED?", count(*) as total from EMPLOYEES
   GROUP by (case 
  when COMMISSION_PCT is  null then 0||'(No)'
  else 1||'(YES)'
  end
);

-- For each job, print the range of salary (difference between minimum and maximum salary) in
-- that job for the jobs that have more than one employee. Order the result by descending
-- range of salary.
-- Example output:

SELECT job_id, max(salary)-min(salary) as range from EMPLOYEES
        group by JOB_ID
        having count(*) > 1
        order by range desc;

-- Show all the employees' employee id, last name and annual salary who are from department
-- 60 or 80 or 100 and have an annual salary greater than 100000 in descending order of their
-- annual salary and then ascending order of their first name.

select employee_id,last_name,salary*12 as annual from EMPLOYEES
        where department_id in(60,80,100) AND salary*12 > 100000
                order by annual desc, FIRST_NAME asc;
