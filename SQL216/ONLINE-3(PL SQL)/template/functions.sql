SUBSTR(string, start, length)
SUBSTR('MMORALES', 1, 2)     -- 'MM' (first 2 chars)
SUBSTR('MMORALES', -2, 2)    -- 'ES' (last 2 chars)
SUBSTR('MMORALES', 3)        -- 'ORALES' (from pos 3 to end)
SUBSTR('MMORALES', -3)       -- 'LES' (last 3 chars)

-- TRAP: -1.2 is NOT valid, use -2,2
SUBSTR(email, -1.2)   -- WRONG
SUBSTR(email, -2, 2)  -- CORRECT

INSTR('Sales Representative', ' ')      -- position of first space = 6
INSTR('Sales Representative', ' ', 1, 2)-- position of 2nd space

-- Used to extract words:
SUBSTR(title, 1, INSTR(title, ' ') - 1)          -- first word
SUBSTR(title, INSTR(title, ' ') + 1)              -- everything after first space

-- Full extracted ID
UPPER(SUBSTR(job_title, 1, 2)) || '_' || 
UPPER(SUBSTR(job_title, INSTR(job_title, ' ') + 1, 3))
-- gives 'SA_REP'
UPPER('sales')        -- 'SALES'
LOWER('SALES')        -- 'sales'
INITCAP('sales rep')  -- 'Sales Rep'

TRIM('  hello  ')     -- 'hello' removes both sides
LTRIM('  hello')      -- 'hello' removes left only
RTRIM('hello  ')      -- 'hello' removes right only
TRIM('x' FROM 'xxhelloxx')  -- 'hello' removes specific char
REPLACE('Hello World', 'World', 'Oracle')  -- 'Hello Oracle'
REPLACE(str, ' ', '_')                     -- replace spaces with underscore



MONTHS_BETWEEN(SYSDATE, HIRE_DATE)        -- positive = hire date is older
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) / 12   -- convert to years
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 60   -- more than 5 years
MONTHS_BETWEEN(SYSDATE, HIRE_DATE) > 12   -- past 1 year probation

-- TRAP: order matters!
MONTHS_BETWEEN(SYSDATE, HIRE_DATE)   -- positive (correct)
MONTHS_BETWEEN(HIRE_DATE, SYSDATE)   -- negative (wrong, common mistake)

ADD_MONTHS(HIRE_DATE, 12)    -- add 1 year to hire date
ADD_MONTHS(SYSDATE, -6)      -- 6 months ago
ADD_MONTHS(HIRE_DATE, n*12)  -- add n years

TRUNC(SYSDATE)                -- removes time, keeps date only
TRUNC(SYSDATE, 'YEAR')        -- Jan 1 of current year
TRUNC(SYSDATE, 'MONTH')       -- 1st of current month

EXTRACT(YEAR FROM HIRE_DATE)    -- gets year number
EXTRACT(MONTH FROM HIRE_DATE)   -- gets month number
EXTRACT(DAY FROM HIRE_DATE)     -- gets day number

-- Example use
WHERE EXTRACT(YEAR FROM HIRE_DATE) = 2005  -- hired in 2005

TO_DATE('2024-01-15', 'YYYY-MM-DD')   -- string to date
TO_CHAR(HIRE_DATE, 'DD-MON-YYYY')     -- date to string
TO_CHAR(HIRE_DATE, 'YYYY')            -- just the year as string
TO_CHAR(SALARY, '99999.99')           -- number to formatted string


SYSDATE - HIRE_DATE          -- difference in DAYS (not months)
SYSDATE - 30                 -- 30 days ago
HIRE_DATE + 365              -- one year after hire

ROUND(AVG(SALARY), 2)    -- round to 2 decimal places
ROUND(4.567, 1)          -- gives 4.6
TRUNC(4.9)               -- gives 4 (cuts, does not round)
TRUNC(6/5)               -- gives 1 (important for bonus calc!)
CEIL(4.1)                -- gives 5 (round up always)
FLOOR(4.9)               -- gives 4 (round down always)

NVL(commission_pct, 0)           -- replace NULL with 0
NVL(manager_id, -1)              -- replace NULL with -1
NVL2(commission_pct, 'HAS', 'NO')-- if not null→first, if null→second
COALESCE(col1, col2, col3, 0)    -- first non-null value

MOD(10, 3)    -- gives 1 (remainder)
MOD(years, n) -- check if evenly divisible


-- Rank with gaps (1,2,2,4)
RANK() OVER (ORDER BY salary DESC)

-- Rank without gaps (1,2,2,3)
DENSE_RANK() OVER (ORDER BY salary DESC)

-- Row number always unique (1,2,3,4)
ROW_NUMBER() OVER (ORDER BY salary DESC)

-- Rank within groups (partition)
RANK() OVER (PARTITION BY department_id ORDER BY salary DESC)

-- Running total
SUM(salary) OVER (ORDER BY employee_id)