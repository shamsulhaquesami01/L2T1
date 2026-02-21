Create table lowest_paid_employees(employee_id varchar2(100), department_id
varchar2(100), saldiff number, bonus number, tag varchar2(100));

CREATE OR REPLACE PROCEDURE UPDATE_LOWEST_PAID_EMPLOYEES(m IN NUMBER, n IN NUMBER) IS
    low_sal              NUMBER;
    maxsal               NUMBER;
    target_id            NUMBER;
    target_id_hire_date  DATE;
    target_id_sal        NUMBER;
    saladiff             NUMBER;
    bonus                NUMBER;
    S_COUNT              NUMBER;
BEGIN
    FOR R IN (SELECT DEPARTMENT_ID FROM DEPARTMENTS) LOOP

        -- Get min and max salary for this department
        SELECT MIN(SALARY), MAX(SALARY) 
        INTO low_sal, maxsal
        FROM EMPLOYEES
        WHERE DEPARTMENT_ID = R.DEPARTMENT_ID;

        -- Skip if no employees
        IF low_sal IS NULL THEN
            CONTINUE;
        END IF;

        -- Case 3: skip if lowest = highest (only one salary level)
        IF low_sal = maxsal THEN
            CONTINUE;
        END IF;

        -- Case 1 and 2: get earliest hired permanent employee with lowest salary
        BEGIN
            SELECT employee_id, hire_date, salary
            INTO target_id, target_id_hire_date, target_id_sal
            FROM (
                SELECT employee_id, hire_date, salary
                FROM EMPLOYEES
                WHERE DEPARTMENT_ID = R.DEPARTMENT_ID
                AND SALARY = low_sal
                AND MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 12  -- permanent only
                ORDER BY HIRE_DATE ASC                        -- earliest joiner
            )
            WHERE ROWNUM = 1;

        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                CONTINUE;  -- no permanent employee found, skip dept
        END;

        -- Calculate saldiff
        saladiff := maxsal - target_id_sal;

        -- Calculate bonus: m per n-year period
        bonus := TRUNC((MONTHS_BETWEEN(SYSDATE, target_id_hire_date) / 12) / n) * m;

        -- Management bonus
        SELECT COUNT(*) INTO S_COUNT 
        FROM EMPLOYEES 
        WHERE MANAGER_ID = target_id;

        IF S_COUNT >= 5 THEN
            bonus := bonus + 5000;
        END IF;

        INSERT INTO LOWEST_PAID_EMPLOYEES 
        VALUES(target_id, R.DEPARTMENT_ID, saladiff, bonus, NULL);

    END LOOP;
    COMMIT;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/

-- Trigger
CREATE OR REPLACE TRIGGER UPDATE_LPET_TAG_TRIGGER
BEFORE INSERT ON LOWEST_PAID_EMPLOYEES
FOR EACH ROW
DECLARE
    global_saladiff NUMBER;
BEGIN
    -- Correct: compute global max saldiff from EMPLOYEES table
    SELECT MAX(MAX_SAL - MIN_SAL) INTO global_saladiff
    FROM (
        SELECT DEPARTMENT_ID,
               MAX(SALARY) AS MAX_SAL,
               MIN(SALARY) AS MIN_SAL
        FROM EMPLOYEES
        GROUP BY DEPARTMENT_ID
    );

    -- Sanity check
    IF :NEW.SALDIFF > global_saladiff THEN
        RAISE_APPLICATION_ERROR(-20001, 'Saldiff value exceeds global maximum.');
    END IF;

    -- Set tag
    IF :NEW.SALDIFF < 10000 THEN
        :NEW.TAG := 'low';
    ELSIF :NEW.SALDIFF < 20000 THEN
        :NEW.TAG := 'very low';
    ELSE
        :NEW.TAG := 'extremely low';
    END IF;

EXCEPTION
    WHEN OTHERS THEN
        RAISE;
END;
/