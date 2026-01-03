select substr(phone_number,instr(PHONE_NUMBER,'.',1,1)+1,instr(PHONE_NUMBER,'.',1,2)-instr(PHONE_NUMBER,'.',1,1)-1) as from Employees;
SELECT phone_number from EMPLOYEES;

select employee_id,salary, salary/350 as re,HIRE_DATE from EMPLOYEES
        where mod(salary/350,2)=0 and to_char(HIRE_DATE,'DD') BETWEEN '01' AND '10';


create table sami_table(
    assignment_id VARCHAR2(20) CONSTRAINT pik PRIMARY KEY,
    EMP_ID NUMBER(6),
    proj_name VARCHAR2(100)  not null ,
    CONSTRAINT b CHECK(proj_name= upper(proj_name)),
    start_date DATE,
    end_date DATE ,
    CONSTRAINT ck check (end_date >= start_date+30),
    CONSTRAINT fk FOREIGN KEY(EMP_ID) references EMPLOYEES(EMPLOYEE_ID) ON DELETE CASCADE
);
drop table sami_table;
COMMIT;

select department_id,sum()
select * from employees;

select substr(last_name,-1,3)||to_char(hire_date,'MM')||abs(length(first_name)-length(last_name)) as hashcode from employees
    where salary >(
    select avg(salary) 
    from Employees
    ) 
    order by hashcode asc;

select substr('Sami Ahmed.Chowdhury', instr('Sami Ahmed Chowdhury',' ')+1, instr('Sami Ahmed Chowdhury',' ',instr('Sami Ahmed Chowdhury',' '),1))
    from dual;
  
