	INPUT default {
		driver = mysql;

		INPUTFIELD VT_TIPO {
			Source = VT.TIPO;
			Type = string;
		}
		INPUTFIELD VT_TIPODOC {
			Source = VT.TIPODOC;
			Type = string;
		}
		INPUTFIELD VT_NUMERO {
			Source = VT.NUMERO;
			Type = string;
		}
		INPUTFIELD VT_FECHA {
			Source = VT.FECHA;
			Type = date;
		}
		INPUTFIELD VT_CANTIDAD {
			Source = VT.CANTIDAD;
			Type = double;
		}
		INPUTFIELD VT_IMPORTE {
			Source = VT.IMPORTE;
			Type = money;
		}
		INPUTFIELD VT_IMPORTECONIVA {
			Source = VT.IMPORTECONIVA;
			Type = money;
		}
		INPUTFIELD VT_PRECIOSINIVA {
			Source = VT.PRECIOSINIVA;
			Type = money;
		}
		INPUTFIELD VT_PRECIOCONIVA {
			Source = VT.PRECIOCONIVA;
			Type = money;
		}
		INPUTFIELD VT_DTOP100 {
			Source = VT.DTOP100;
			Type = double;
		}
		INPUTFIELD VT_TIPOIVA_ID {
			Source = VT.TIPOIVA_ID;
			Type = integer;
		}
		INPUTFIELD VT_TIPOIVA_CODIGO {
			Source = VT.TIPOIVA_CODIGO;
			Type = integer;
		}
#ifdef NOTAS
		INPUTFIELD VT_NOTAS {
			Source = VT.NOTAS;
			Type = text;
		}
#endif
		INPUTFIELD ARTICULO_ID {
			Source = A.ID;
			Type = string;
		}
		INPUTFIELD ARTICULO_CODIGO {
			Source = A.CODIGO;
			Type = string;
		}
		INPUTFIELD ARTICULO_NOMBRE {
			Source = A.NOMBRE;
			Type = string;
		}
		INPUTFIELD RAZONSOCIAL {
			Source = VT.RAZONSOCIAL;
			Type = string;
		}
		INPUTFIELD RAZONSOCIAL_CODIGO {
			Source = VT.RAZONSOCIAL_CODIGO;
			Type = integer;
		}
#ifdef AGRUPAR_POR_DOCUMENTO
		INPUTFIELD DOCUMENTO_GRUPO {
			Source = CONCAT(VT.FECHA,VT.NUMERO);
			Type = string;
		}
#endif
	}

	include = "default_pageheader.inc";

#ifdef AGRUPAR
	GROUPHEADER grupo {
		level = 1;
		sizey = 1.2;
		fontweight = "bold";
		fontcolor = "blue";

#ifdef AGRUPAR_POR_ARTICULO
		value = "=input.ARTICULO_CODIGO";
		OBJECT articulo_codigo {
			value = "=input.ARTICULO_CODIGO";
			posx = 2;
			posy = 0.2;
			sizex = 25;
		}
		OBJECT articulo_nombre {
			value = "=input.ARTICULO_NOMBRE";
			posx = 30;
			posy = 0.2;
			sizex = 50;
		}
#endif
#ifdef AGRUPAR_POR_RAZONSOCIAL
		value = "=input.RAZONSOCIAL_CODIGO";
		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = 2;
			posy = 0.2;
			sizex = 8;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL";
			posx = 11;
			posy = 0.2;
			sizex = 0;
		}
#endif
#ifdef AGRUPAR_POR_FECHA_DIARIO
		value = "=input.VT_FECHA";
		OBJECT fecha_caption {
			value = "= const @DDD.FACTURAVENTA_FECHA_CAPTION;";
			posx = 0;
			posy = 0;
			sizex = 10;
		}
		OBJECT fecha {
			value = "=input.VT_FECHA";
			posx = -1; // Ok
			posy = 0;
		}
#endif // AGRUPAR_POR_FECHA_DIARIO
#ifdef AGRUPAR_POR_DOCUMENTO
		value = "=input.DOCUMENTO_GRUPO";
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = 0;
			sizex = 2;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			sizex = 14;
			posx = 3;
		}
		OBJECT numero {
			value = "=input.VT_NUMERO";
			posx = 18;
			sizex = 20;
			posy = 0;
		}
		OBJECT fecha {
			value = "=input.VT_FECHA";
			posx = 39;
			sizex = 10;
			posy = 0;
		}
		OBJECT RAZONSOCIAL_CODIGO {
			value = "=input.RAZONSOCIAL_CODIGO";
			alignment = right;
			sizex = 4;
			posx = 50;
		}
		OBJECT RAZONSOCIAL {
			value = "=input.RAZONSOCIAL";
			adjustment = reducefont;
			posx = 55;
		}

#endif // AGRUPAR_POR_DOCUMENTO
	}
#endif // AGRUPAR

#ifndef AGRUPAR
	PAGEHEADER header_captions {
		borderstyle = solid;
		bordercolor = black;
		fontweight = bold;
#endif
#ifdef AGRUPAR
	GROUPHEADER header_captions {
		fontitalic = true;
		bordertopstyle=solid;
		bordertopwidth=1;
		borderbottomstyle=solid;
		borderbottomwidth=1;
		fontsize = -2;
#endif
		sizey = 1.3;
		style = header;
		level = 1;

#ifndef AGRUPAR_POR_FECHA_DIARIO
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_FECHA {
			value = "= const @DDD.ALBARANVENTA_FECHA_CAPTION;";
			posx = 0;
			sizex = 7;
		}
#endif
#endif
#ifdef AGRUPAR_POR_DOCUMENTO
// Como no se imprimen los primeros campos, se desplaza a la derecha
	posx = -28;
#endif	
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_TIPODOC {
			value = "= const @DDD.TIPODOC_DESCSINGULAR;";
			posx = 7;
			adjustment = reducefont;
			sizex = 11;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 0;
#endif
		}
		OBJECT VT_NUMERO {
			adjustment = reducefont;
			value = "= const @DDD.ALBARANVENTA_NUMERO_CAPTION;";
			posx = 19;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 12;
#endif
			sizex = 11;
		}
#endif
		OBJECT VT_CANTIDAD {
			value = "= const @DDD.ALBARANVENTADET_CANTIDAD_CAPTION;";
			posx = 29;
			sizex = 7;
			alignment = right;
		}
		OBJECT VT_PRECIO {
#ifdef PRECIOS_CON_IVA
			value = "Precio IVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "Precio";
#endif
			posx = 37;
			sizex = 8;
			alignment = right;
		}
		OBJECT VT_DTOP100 {
			value = "= const @DDD.ALBARANVENTADET_DTOP100_CAPTION;";
			posx = 46;
			sizex = 5;
			alignment = right;
		}
		OBJECT VT_IMPORTE {
			value = "= const @DDD.ALBARANVENTADET_IMPORTE_CAPTION;";
			posx = 52;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_TIPOIVA_CODIGO {
			value = "IVA";
			posx = 63;
			sizex = 3;
			alignment = right;
		}
#ifndef AGRUPAR_POR_ARTICULO
		OBJECT ARTICULO_NOMBRE {
			value = "= const @DDD.ARTICULO_DESCSINGULAR;";
			posx = 67;
			sizex = 20;
		}
#endif
#ifdef AGRUPAR_POR_FECHA_DIARIO
		OBJECT ARTICULO_NOMBRE {
			sizex = 20;
		}
#endif
#ifndef AGRUPAR_POR_RAZONSOCIAL
		OBJECT CLIENTE_RAZONSOCIAL {
			value = "= const @DDD.CLIENTE_RAZONSOCIAL_CAPTION;";
			posx = 70;
#ifndef AGRUPAR_POR_ARTICULO
			posx = 98;
#endif
		}
#endif
	}


	DETAILS details {
		level = 0;
		style = detail;
		sizey = 1;

#ifndef AGRUPAR_POR_FECHA_DIARIO
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_FECHA {
			value = "=input.VT_FECHA";
			posx = 0;
			sizex = 7;
		}
#endif
#endif
#ifdef AGRUPAR_POR_DOCUMENTO
// Como no se imprimen los primeros campos, se desplaza a la derecha
	posx = -28;
#endif	
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = 7;
#ifdef AGRUPAR // Agrupado no es apaisado			
			posx = 8;
#endif			
			sizex = 2;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 0;
#endif
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			sizex = 8;
			posx = -0.5;
			adjustment = reducefont;
		}
		OBJECT VT_NUMERO {
			value = "=input.VT_NUMERO";
			posx = 18;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 12;
#endif
			sizex = 11;
			adjustment = reducefont;
		}
#endif
		OBJECT VT_CANTIDAD {
			value = "=input.VT_CANTIDAD";
			posx = 30;
			sizex = 6;
			alignment = right;
			adjustment = reducefont;
		}
		OBJECT VT_PRECIOCONIVA {
			value = "=input.VT_PRECIOSINIVA";
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_PRECIOCONIVA";
#endif
			posx = 37;
			sizex = 8;
			alignment = right;
		}

		OBJECT VT_DTOP100 {
			value = "=input.VT_DTOP100";
			posx = 46;
			sizex = 5;
			alignment = right;
			mask = "990.99";
		}
		OBJECT VT_IMPORTE {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTECONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTE";
#endif
			posx = 52;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_TIPOIVA_CODIGO {
			value = "=input.VT_TIPOIVA_CODIGO";
			posx = 63;
			sizex = 3;
			alignment = right;
		}
#ifndef AGRUPAR_POR_ARTICULO
		OBJECT ARTICULO_CODIGO {
			value = "=input.ARTICULO_CODIGO";
			supressdup = true;
			posx = 67;
			sizex = 30;
			fontsize = -2;
			adjustment = reducefont;
			fontweight = bold;
		}
		OBJECT ARTICULO_NOMBRE {
			value = "=input.ARTICULO_NOMBRE";
			supressdup = true;
			posx = -0.5; 
			sizex = 21;
			fontsize = -2;
			adjustment = reducefont;
		}
#endif
#ifdef AGRUPAR_POR_FECHA_DIARIO
		OBJECT ARTICULO_NOMBRE {
			sizex = 20;
			adjustment = reducefont;
		}
#endif
#ifndef AGRUPAR_POR_RAZONSOCIAL
		OBJECT RAZONSOCIAL_CODIGO {
			value = "= @input.RAZONSOCIAL_CODIGO";
#ifndef AGRUPAR			
			fontsize = -2;
#endif			
			supressdup = true;
			alignment = left;
			sizex = 5;
			posx = 70;
			fontweight = bold;
			paddingleft = 0.2;
#ifndef AGRUPAR_POR_ARTICULO
			posx = 98;
#endif
		}
		OBJECT RAZONSOCIAL {
			value = "=input.RAZONSOCIAL";
#ifndef AGRUPAR			
			fontsize = -2;
#endif			
			supressdup = true;
			posx = -0.5;
		}
#endif
	}

#ifdef NOTAS
// Las notas van en una línea aparte
	DETAILS details_notas {
		style = detail;
		sizey = 1;
		supifblank = true;

		OBJECT VT_NOTAS {
			value = "=input.VT_NOTAS";
			alignment = right;
			posx = 0;
		}
	}
#endif

#ifdef AGRUPAR
	GROUPFOOTER grupo_footer {
		level = 1;
		sizey = 1.2;
		fontweight = "bold";
		margintop = 0.3;
		bordertopstyle = solid;
		bordertopwidth = 1;
#ifdef AGRUPAR_POR_DOCUMENTO
// Como no se imprimen los primeros campos, se desplaza a la derecha
	posx = -28;
#endif	

		OBJECT VT_CANTIDAD_grupo {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 27;
			sizex = 9;
			alignment = right;
		}
		OBJECT VT_IMPORTE_grupo {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTECONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTE";
#endif
			aggregate = sum;
			posx = 50;
			sizex = 12;
			alignment = right;
		}
	} // group footer
#endif // AGRUPAR


	REPORTFOOTER report_footer
	{
		sizey = 1.2;
		fontweight = bold;
		bordertopstyle = solid;
		bordertopwidth = 1;
		margintop = 0.2;

		OBJECT articulo_totales {
			value = "Totales: ";
			posx = 2;
			sizex = 8;
		}
		OBJECT VT_CANTIDAD_total {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 26;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_IMPORTE_total {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTECONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_IMPORTE";
#endif
			aggregate = sum;
			posx = 50;
			sizex = 12;
			alignment = right;
		}
	}
