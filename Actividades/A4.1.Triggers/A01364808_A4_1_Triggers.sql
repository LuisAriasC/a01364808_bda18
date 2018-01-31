create trigger update_aumento before update on EMPLOYEE referencing old as old_values new as new_values for each row mode db2sql when (new_values.salary > old_values.salary * 1.3) begin atomic set new_values.salary = old_values.salary; end


create trigger update_inventario after insert on VENTAS referencing new as new_value for each row mode db2sql when (new_value-cantidad > (select QUANTITY from INVENTORY when pid like new_value.ID)) begin atomic signal sqlstate '75001' ('error'); end


create trigger delivered after updateof ID on VENTAS referencing new as new_value for each row mode db2sql when (new_value.estado = 1) begin atomic update INVENTORY set QUANTITY = QUANTITY - new_value.cantidad; end
