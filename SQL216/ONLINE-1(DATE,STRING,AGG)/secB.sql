select substr(PHONE_NUMBER, 1,3) as code,count(*) as numbers from employees
    where HIRE_DATE - TO_DATE('01-JAN-2005') >=0
        GROUP by substr(PHONE_NUMBER, 1,3);

select employee_id,salary, abs(salary-(round((salary/1000))*1000)) as diff from EMPLOYEES;

SELECT department_id, TO_CHAR(sum(salary*12),'99,999,999') as annual from EMPLOYEES
        where SALARY >=2500
        GROUP BY DEPARTMENT_ID
            having count(*)>2;
create table emp_training(
    training_id VARCHAR2(20),
    employee_id NUMBER,
    training_name VARCHAR2(50) CONSTRAINT pk PRIMARY KEY,
    reg_date DATE default SYSDATE,
    CONSTRAINT  fk FOREIGN KEY(employee_id) references employees(employee_id)

);
INSERT INTO EMP_TRAINING VALUES('10763562',102,'sami','21-JAN-2025');
INSERT INTO EMP_TRAINING VALUES('10763562',100,'rahim','21-JAN-2025');
COMMIT;

select employee_id, substr(first_name,1,2)||length(last_name)||substr(PHONE_NUMBER,length(PHONE_NUMBER),1) as security
    from EMPLOYEES
    where length(FIRST_NAME) > length(last_name)
    order by length(security) desc;

