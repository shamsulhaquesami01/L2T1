select d.department_id,d.department_name,(
    select avg(salary) from EMPLOYEES e
    where e.department_id=d.department_id
    ),(
        (select count(*) from employees e1
        where e1.SALARY >= (
    select avg(salary) from EMPLOYEES e2
    where e1.department_id=e2.department_id
    ) and e1.department_id=d.DEPARTMENT_ID )
 /
nullif((select count(*) from employees e1
        where e1.SALARY <= (
    select avg(salary) from EMPLOYEES e2
    where e1.department_id=e2.department_id
    )and e1.department_id=d.DEPARTMENT_ID ),0)

       )   as ratio
from DEPARTMENTS d;




SELECT d.department_id, 
       d.department_name, 
       da.avg_salary,
       -- Logic: Count Above / Count Below
       (SELECT COUNT(*) FROM employees e1 
        WHERE e1.department_id = d.department_id AND e1.salary >= da.avg_salary) 
        / 
        NULLIF((SELECT COUNT(*) FROM employees e2 
                WHERE e2.department_id = d.department_id AND e2.salary <= da.avg_salary), 0) AS ratio
FROM departments d
JOIN (
    -- Get the averages once, so we don't recalculate 100 times
    SELECT department_id, AVG(salary) AS avg_salary
    FROM employees
    GROUP BY department_id
) da ON d.department_id = da.department_id;