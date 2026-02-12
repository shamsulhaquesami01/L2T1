create or replace function is_ready_for_promotion(eid in number) 
return  varchar2 is
id number;
BEGIN
    for R in (select e.employee_id  from employees e 
    where MONTHS_BETWEEN(sysdate,e.HIRE_DATE) >=60 AND
    e.salary > (select (j.min_salary+j.max_salary)/2 from jobs j where e.job_id=j.job_id) and
    1<=(select count(e2.employee_id) from employees e2 where e2.MANAGER_ID=e.employee_id))
    loop
 IF eid = R.employee_id THEN 
            RETURN 'yes';
        END IF;
    END LOOP;
    RETURN 'no';
EXCEPTION
WHEN NO_DATA_FOUND THEN
RETURN 'No employee found.' ;
when others THEN
return 'i donnnn know';
end;
/


DECLARE
msg varchar2(100);
BEGIN
    msg :=IS_READY_FOR_PROMOTION(100);
    DBMS_OUTPUT.PUT_LINE('Function status: ' || msg);
END;
/

SELECT EMPLOYEE_ID,COMMISSION_PCT from EMPLOYEES_COPY;


create or replace PROCEDURE update_salary IS
newsewt date;
BEGIN
    select max(hire_date) into newsewt from EMPLOYEES;
    update EMPLOYEES_COPY e
    set e.salary =
    CASE
   when ( e.SALARY + (NVL(e.COMMISSION_PCT,0) * e.SALARY) + 0.1 * (select j.min_salary from jobs j
    where j.job_id=e.JOB_ID) + 0.1 * (select avg(e2.salary) from EMPLOYEES e2 where e.DEPARTMENT_ID=e2.DEPARTMENT_ID and e.DEPARTMENT_ID is not null)>
    (select j.max_salary from jobs j where j.job_id=e.JOB_ID)) 
    then (select j.max_salary from jobs j where j.job_id=e.JOB_ID)
    else  e.SALARY + (NVL(e.COMMISSION_PCT,0) * e.SALARY) + 0.1 * (select j.min_salary from jobs j
    where j.job_id=e.JOB_ID) + 0.1 * (select avg(e2.salary) from EMPLOYEES e2 where e.DEPARTMENT_ID=e2.DEPARTMENT_ID and e.DEPARTMENT_ID is not null)
    end
    where 12 <= MONTHS_BETWEEN(newsewt ,e.hire_date);
    end;
    /

execute UPDATE_SALARY;

SELECT e.employee_id, e.salary as Old_Salary, ec.salary as New_Salary,
j.min_salary, j.max_salary, e.hire_date
FROM employees_copy ec join employees e
on ec.employee_id=e.employee_id
join jobs j on e.job_id=j.job_id;