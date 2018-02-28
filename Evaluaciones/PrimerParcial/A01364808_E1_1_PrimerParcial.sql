-- PROBLEMA 1

-- CREA TABLA LOG_FILM
create table LOG_FILM(Id int PRIMARY KEY NOT NULL AUTO_INCREMENT, Tipo varchar(10) NOT NULL DEFAULT 'Update', Film_id smallint(5), Last_value varchar(50), New_value varchar(50), Lats_update TIMESTAMP DEFAULT CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP);





-- CREA PROCEDURE PARA INSERTAR VALORES EN LOG_FILM
DELIMITER // 
CREATE procedure actu_LOG_FILM(
    in tipo_cambio varchar(10),
    in film_id smallint(5),
    in last_value varchar(50), 
    in new_value varchar(50))
BEGIN
    declare cambio varchar(10);
    declare lastval varchar(20);
    declare newval varchar(20);
    
    set cambio = concat(tipo_cambio, '%');
    set lastval = concat(last_value, '%');
    set newval = concat(new_value, '%');
    
    insert into LOG_FILM (Tipo, film_id, last_value, new_value) values (cambio, film_id, lastval, newval);

END //
DELIMITER ;



--CREA TRIGGER PARA HACER EL AUTOGUARDADO DEL UPDATE
DELIMITER //
CREATE TRIGGER update_film AFTER UPDATE ON film 
    FOR EACH ROW 
BEGIN
    
	CALL actu_LOG_FILM('Update', OLD.Film_id, 'OLD' , 'NEW');
END //
DELIMITER;



--PROCEDURE Y CURSORES PARA PELÍCULAS
delimiter //
CREATE PROCEDURE film_language()
BEGIN
  DECLARE ids int;
  DECLARE done INT DEFAULT FALSE;
  DECLARE cursor1 CURSOR FOR SELECT film_id FROM film;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  OPEN cursor1;
  read_loop: LOOP
    IF done THEN
      LEAVE read_loop;
    END IF;
		
    FETCH cursor1 INTO ids;
    UPDATE film as a JOIN film_category as b ON a.film_id = b.film_id JOIN category as c on b.category_id = c.category_id JOIN language 
	    SET 
            SET title = concat(c.name,"_",title) WHERE a.film_id = ids;
    END LOOP;
    CLOSE cursor1;
END//
delimiter;





-- PROBLEMA 2

-- CREAR TABLA DE GOMITAS

CREATE TABLE gomitas(ID INT NOT NULL GENERATED ALWAYS AS IDENTITY (START WITH 1, INCREMENT BY 1),NAME VARCHAR(50) NOT NULL,PRICE DECIMAL(10,6) NOT NULL,cstart DATE NOT NULL,cend DATE NOT NULL,period business_time(cstart, cend),PRIMARY KEY(ID, business_time WITHOUT overlaps))

INSERT INTO gomitas (id, name, price, cstart, cend) VALUES
  ('clasicas', 10, '2018-1-1', '2019-1-1'),
  ('blancas', 20, '2018-1-1', '2019-1-1'),
  ('espolvoreada', 30, '2018-1-1', '2019-1-1'),
  ('gomitas', 15, '2018-1-1', '2019-1-1'),
  ('corazon', 25, '2018-1-1', '2019-1-1'),
  ('dientes', 35, '2018-1-1', '2019-1-1'),
  ('perrito', 12, '2018-1-1', '2019-1-1'),
  ('manzanita', 22, '2018-1-1', '2019-1-1'),
  ('dinosaurio', 32, '2018-1-1', '2019-1-1'),
  ('pandita', 13, '2018-1-1', '2019-1-1'),
  ('gusanito', 23, '2018-1-1', '2019-1-1'),
  ('dulcigoma', 33, '2018-1-1', '2019-1-1');

CREATE PROCEDURE costos() BEGIN UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-2-1' to '2019-1-1' SET PRICE = PRICE*1.45; UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-2-15' to '2019-1-1' SET PRICE = (PRICE/1.45)*1.1; UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-4-25' to '2019-1-1' SET PRICE = PRICE*1.45; UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-5-5' to '2019-1-1' SET PRICE = (PRICE/1.45)*1.1; UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-10-25' to '2019-1-1' SET PRICE = PRICE*1.45; UPDATE gomitas FOR PORTION OF BUSINESS_TIME FROM '2018-11-5' to '2019-1-1' SET PRICE = (PRICE/1.45)*1.1; END


call costos

SELECT * from gomitas WHERE NAME='dientes';

SELECT SUM(PRICE)/COUNT(*) as AVG from gomitas WHERE NAME='dientes';

SELECT MAX(PRICE) as MAX from gomitas WHERE NAME='dientes';

SELECT MIN(PRICE) as MAX from gomitas WHERE NAME='dientes';
