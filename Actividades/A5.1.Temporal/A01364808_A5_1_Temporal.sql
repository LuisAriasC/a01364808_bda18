create table Usuarios(ID int PRIMARY KEY not null,Nombre varchar(20) not null,direccion varchar(50) not null,Correo varchar(40), año int,modelo varchar(15),marca varchar(15),precio int,num_serie int,SYS_START TIMESTAMP(12) GENERATED ALWAYS AS ROW BEGIN NOT NULL,SYS_END TIMESTAMP(12) GENERATED ALWAYS AS ROW END NOT NULL,TRANS_START TIMESTAMP(12) GENERATED ALWAYS AS TRANSACTION START ID IMPLICITLY HIDDEN, PERIOD SYSTEM_TIME(SYS_START,SYS_END))

create table Polizas(ID int PRIMARY KEY not null, ID_Usuario int,FOREIGN KEY (ID_Usuario) REFERENCES Usuarios(ID),fecha_inicio date not null, fecha_fin date not null,cobertura smallint not null,costo int not null,SYS_START TIMESTAMP(12) GENERATED ALWAYS AS ROW BEGIN NOT NULL,SYS_END TIMESTAMP(12) GENERATED ALWAYS AS ROW END NOT NULL,TRANS_START TIMESTAMP(12) GENERATED ALWAYS AS TRANSACTION START ID IMPLICITLY HIDDEN, PERIOD SYSTEM_TIME(SYS_START,SYS_END))

create table Usuarios_hist like Usuarios

alter table Usuarios add versioning use HISTORY table Usuarios_hist 

create table Polizas_hist like Polizas

alter table Polizas add versioning use HISTORY table Polizas_hist

Create TRIGGER Caneclacion BEFORE update on Polizas referencing old as old_values new as new_values for each row mode db2sql when (new_values.fecha_fin < old_values.fecha_fin) begin atomic set new_values.costo = (old_values.costo / 2); end

CREATE OR REPLACE TRIGGER Ampliar_cobertura BEFORE update on Polizas referencing old as old_values new as new_values for each row mode db2sql when (old_values.cobertura <= 4 and new_values.cobertura > 4 and new_values.cobertura <= old_values.cobertura) begin atomic set new_values.cobertura = old_values.cobertura; end

INSERT INTO Usuarios (ID,Nombre,direccion) VALUES (1,'Luis','Colorines')
INSERT INTO Usuarios (ID,Nombre,direccion) VALUES (2,'Alex','Alemndros')
INSERT INTO Usuarios (ID,Nombre,direccion) VALUES (3,'Natalia','Sirenas')

INSERT INTO Polizas (ID,ID_Usuario,fecha_inicio, fecha_fin,cobertura,costo) VALUES (10,2,'2017-09-01', '2018-09-01',1,1000)
INSERT INTO Polizas (ID,ID_Usuario,fecha_inicio, fecha_fin,cobertura,costo) VALUES (20,1,'2017-12-01', '2018-12-01',3,3000)
INSERT INTO Polizas (ID,ID_Usuario,fecha_inicio, fecha_fin,cobertura,costo) VALUES (30,3,'2017-06-01', '2018-06-01',4,4000)

UPDATE Polizas SET cobertura = 3 where ID_Usuario = 2
UPDATE Polizas SET cobertura = 2 where ID_Usuario = 1
UPDATE Polizas SET cobertura = 4 where ID_Usuario = 3

UPDATE Polizas SET fecha_fin = '2018-01-01' where ID_Usuario = 3
