select textDescription from customer c, orders o, orderdetails od, products p, productlines, pl, where c.customersnumber = 112 and c.customerNumber = o.customerNumber and o.orderNumber = od.orderNumber and od.productCode = p.productCode and p.productCode = pl.productLine

