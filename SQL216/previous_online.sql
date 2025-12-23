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


