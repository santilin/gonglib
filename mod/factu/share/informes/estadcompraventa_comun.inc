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
	}

	include = "default_pageheader.inc";

#ifdef AGRUPAR
	GROUPHEADER grupo {
		level = 1;
		sizey = 1.2;
		fontweight = "bold";
		margintop = 0.5;
		fontcolor = "blue";

#ifdef AGRUPAR_POR_ARTICULO
		value = "=input.ARTICULO_CODIGO";
		OBJECT articulo_codigo {
			value = "=input.ARTICULO_CODIGO";
			posx = 2;
			posy = 0.2;
			sizex = 20;
		}
		OBJECT articulo_nombre {
			value = "=input.ARTICULO_NOMBRE";
			posx = -1;
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
			posx = -1;
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
			posx = -1;
			posy = 0;
		}
#endif // AGRUPAR_POR_FECHA_DIARIO
#ifdef AGRUPAR_POR_DOCUMENTO
		value = "=input.VT_NUMERO";
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = 0;
			sizex = 2;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			sizex = 14;
			posx = -1;
		}
		OBJECT numero {
			value = "=input.VT_NUMERO";
			posx = -1;
			sizex = 20;
			posy = 0;
		}
		OBJECT fecha {
			value = "=input.VT_FECHA";
			posx = -1;
			sizex = 10;
			posy = 0;
		}
		OBJECT RAZONSOCIAL_CODIGO {
			value = "=input.RAZONSOCIAL_CODIGO";
			alignment = right;
			sizex = 4;
			posx = -1;
		}
		OBJECT RAZONSOCIAL {
			value = "=input.RAZONSOCIAL";
			adjustment = reducefont;
			posx = -1;
		}

#endif // AGRUPAR_POR_DOCUMENTO
	}
#endif // AGRUPAR

#ifndef AGRUPAR
	PAGEHEADER header_captions {
		borderbottomstyle = solid;
		borderbottomcolor = black;
		marginbottom = .4;
		fontweight = bold;
		sizey = 1.3;
#endif
#ifdef AGRUPAR
	GROUPHEADER header_captions {
		fontitalic = "true";
		sizey = 1.1;
#endif
		style = header;
		level = 1;
		paddingtop = 0.1;

#ifndef AGRUPAR_POR_FECHA_DIARIO
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_FECHA {
			value = "= const @DDD.ALBARANVENTA_FECHA_CAPTION;";
			posx = 0;
			sizex = 7;
		}
#endif
#endif
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_TIPODOC {
			value = "= const @DDD.TIPODOC_DESCSINGULAR;";
			posx = -1;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 0;
#endif
			sizex = 6;
		}
		OBJECT VT_NUMERO {
			value = "= const @DDD.ALBARANVENTA_NUMERO_CAPTION;";
			posx = -1;
			sizex = 8;
		}
#endif
		OBJECT VT_CANTIDAD {
			value = "= const @DDD.ALBARANVENTADET_CANTIDAD_CAPTION;";
			posx = -1;
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
			posx = -1;
			sizex = 8;
			alignment = right;
		}
		OBJECT VT_DTOP100 {
			value = "= const @DDD.ALBARANVENTADET_DTOP100_CAPTION;";
			posx = -1;
			sizex = 5;
			alignment = right;
		}
		OBJECT VT_IMPORTE {
			value = "= const @DDD.ALBARANVENTADET_IMPORTE_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_TIPOIVA_CODIGO {
			value = "IVA";
			posx = -1;
			sizex = 3;
			alignment = right;
		}
#ifndef AGRUPAR_POR_ARTICULO
		OBJECT ARTICULO_CODIGO {
			value = "= const @DDD.ARTICULO_CODIGO_CAPTION;";
			posx = -1;
			sizex = 10;
		}
		OBJECT ARTICULO_NOMBRE {
			value = "= const @DDD.ARTICULO_NOMBRE_CAPTION;";
			posx = -1;
		}
#endif
#ifndef AGRUPAR
		OBJECT ARTICULO_NOMBRE {
			sizex = 25;
		}
#endif
#ifdef AGRUPAR_POR_RAZONSOCIAL
		OBJECT ARTICULO_NOMBRE {
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
			posx = -1;
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
#ifndef AGRUPAR_POR_DOCUMENTO
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = -1;
			sizex = 2;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 0;
#endif
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			sizex = 4;
#ifdef AGRUPAR_POR_FECHA_DIARIO
			posx = 2;
#endif
#ifndef AGRUPAR_POR_FECHA_DIARIO
			posx = 10;
#endif
		}
		OBJECT VT_NUMERO {
			value = "=input.VT_NUMERO";
			posx = -1;
			sizex = 8;
			adjustment = reducefont;
		}
#endif
		OBJECT VT_CANTIDAD {
			value = "=input.VT_CANTIDAD";
			posx = -1;
			sizex = 7;
			alignment = right;
		}
		OBJECT VT_PRECIOCONIVA {
#ifdef PRECIOS_CON_IVA
			value = "=input.VT_PRECIOCONIVA";
#endif
#ifndef PRECIOS_CON_IVA
			value = "=input.VT_PRECIOSINIVA";
#endif
			posx = -1;
			sizex = 8;
			alignment = right;
		}

		OBJECT VT_DTOP100 {
			value = "=input.VT_DTOP100";
			posx = -1;
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
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_TIPOIVA_CODIGO {
			value = "=input.VT_TIPOIVA_CODIGO";
			posx = -1;
			sizex = 3;
			alignment = right;
		}
#ifndef AGRUPAR_POR_ARTICULO
		OBJECT ARTICULO_CODIGO {
			value = "=input.ARTICULO_CODIGO";
			supressdup = true;
			posx = -1;
			sizex = 10;
		}
		OBJECT ARTICULO_NOMBRE {
			value = "=input.ARTICULO_NOMBRE";
			supressdup = true;
			posx = -1; // Tomar el mayor tamaño posible. Si luego hay que poner la razón social detrás, ya se cambiará el tamaño
		}
#endif
#ifndef AGRUPAR
		OBJECT ARTICULO_NOMBRE {
			sizex = 25;
		}
#endif
#ifdef AGRUPAR_POR_RAZONSOCIAL
		OBJECT ARTICULO_NOMBRE {
			sizex = 20;
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
			value = "=input.RAZONSOCIAL_CODIGO";
			supressdup = true;
			alignment = right;
			sizex = 4;
			posx = -1;
		}
		OBJECT RAZONSOCIAL {
			value = "=input.RAZONSOCIAL";
			adjustment = reducefont;
			supressdup = true;
			posx = -1;
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

		OBJECT VT_CANTIDAD_grupo {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 19;
			sizex = 12;
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
			posx = 45;
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
		margintop = 0.2;
		paddingtop = 0.6;

		OBJECT articulo_totales {
			value = "Totales: ";
			posx = 2;
			sizex = 8;
		}
		OBJECT VT_CANTIDAD_total {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 19;
			sizex = 12;
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
			posx = 45;
			sizex = 12;
			alignment = right;
		}
	}
