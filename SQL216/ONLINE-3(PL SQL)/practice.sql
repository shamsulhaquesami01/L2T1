select * from employees;
select * from departments;
select * from locations;
select * from countries;
select * from regions;


create or replace procedure LSE(jayga in varchar2) is
eid number;
begin
select e.employee_id into eid from employees e join departments d on e.DEPARTMENT_ID=d.DEPARTMENT_ID
join locations l on d.LOCATION_ID=l.LOCATION_ID
 join countries c on l.country_id=c.country_id
 join regions r on r.region_id=c.region_id
where r.region_name=jayga and
e.salary
;
select first_name||last_name from employees where employee_id=eid;
end;
/

execute LSE('Americas');
