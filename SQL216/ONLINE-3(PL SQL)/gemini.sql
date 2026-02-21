create or replace trigger trg_salary_equity
before update of salary 
on EMPLOYEES for each row
declare
manager_sal number;
lowest_salary number;
begin
    select salary into manager_sal from employees where employee_id=:new.manager_id;
    if(:new.salary > manager_sal) then
    RAISE_APPLICATION_ERROR(-20001,' salary too high');
    end if;
    select salary into lowest_salary from employees where 0=(
        SELECT count(*) from employees e where e.salary < salary and e.department_id=department_id
    );
    if(:old.salary=lowest_salary) then 
        if((:new.salary-:old.salary)/:old.salary <0.15) then RAISE_APPLICATION_ERROR(-20001, 'increament not enough');
        end if;
    end if;
    if(:old.department_id=90) then
        if(MONTHS_BETWEEN(sysdate,:old.hire_date)<60) then RAISE_APPLICATION_ERROR(-20001, 'time not enough');
        end if;
    end if;
end;
/

