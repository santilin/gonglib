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

	PAGEHEADER header_captions {
		style = header;
		sizey = 1;
		fontweight = "bold";
		borderstyle = solid;
		borderwidth = 1;

#ifdef AGRUPAR_POR_FECHA_DIARIO
		OBJECT fecha_caption {
			value = "= const @DDD.FACTURAVENTA_FECHA_CAPTION;";
			posx = 0;
			posy = 0;
			sizex = 10;
		}
#endif
#ifdef AGRUPAR_POR_ARTICULO
		OBJECT articulo_codigo {
			value = "=const @DDD.ARTICULO_CODIGO_CAPTION;";
			posx = 0;
			posy = 0;
			sizex = 20;
		}
		OBJECT articulo_nombre {
			value = "=const @DDD.ARTICULO_DESCSINGULAR;";
			posx = 21;
			posy = 0;
			sizex = 30;
		}
#endif
#ifdef AGRUPAR_POR_RAZONSOCIAL
		OBJECT articulo_codigo {
			value = "=const @DDD.CLIENTE_CODIGO_CAPTION;";
			posx = 0;
			posy = 0;
			sizex = 6;
		}
		OBJECT articulo_nombre {
			value = "=const @DDD.CLIENTE_DESCSINGULAR;";
			posx = 7;
			posy = 0;
			sizex = 48;
		}
#endif
#ifdef AGRUPAR_POR_DOCUMENTO
		value = "=input.DOCUMENTO_GRUPO";
		OBJECT fecha {
			value = "=const @DDD.ALBARANVENTA_FECHA_CAPTION;";
			posx = 0;
			sizex = 6;
			posy = 0;
		}
		OBJECT VT_TIPO {
			value = "=const @DDD.TIPODOC_DESCSINGULAR";
			posx = 7;
			sizex = 10;
		}
		OBJECT numero {
			value = "=const @DDD.ALBARANVENTA_NUMERO_CAPTION;";
			posx = -1;
			sizex = 12;
			posy = 0;
		}
		OBJECT RAZONSOCIAL_CODIGO {
			value = "=const @DDD.CLIENTE_DESCSINGULAR;";
			sizex = 18;
			posx = -1;
		}
#endif // AGRUPAR_POR_DOCUMENTO
		OBJECT VT_CANTIDAD {
			value = "= const @DDD.ALBARANVENTADET_CANTIDAD_CAPTION;";
			posx = 75;
			sizex = 12;
			alignment = right;
		}
		OBJECT VT_IMPORTE {
			value = "= const @DDD.ALBARANVENTADET_IMPORTE_CAPTION;";
			posx = 88;
			sizex = 12;
			alignment = right;
		}
	}


#ifdef AGRUPAR
	GROUPFOOTER grupo_footer {
		level = 1;
		sizey = 1;
		fontweight = "bold";

#ifdef AGRUPAR_POR_ARTICULO
		value = "=input.ARTICULO_CODIGO";
		OBJECT articulo_codigo {
			value = "=input.ARTICULO_CODIGO";
			posx = 0;
			posy = 0;
			sizex = 20;
		}
		OBJECT articulo_nombre {
			value = "=input.ARTICULO_NOMBRE";
			posx = 21;
			posy = 0;
			sizex = 47;
			adjustment = grow;
		}
#endif // AGRUPAR_POR_ARTICULO
#ifdef AGRUPAR_POR_RAZONSOCIAL
		value = "=input.RAZONSOCIAL_CODIGO";
		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = 0;
			posy = 0;
			sizex = 6;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL";
			posx = 7;
			posy = 0;
			sizex = 49;
		}
#endif // AGRUPAR_POR_RAZONSOCIAL
#ifdef AGRUPAR_POR_FECHA_DIARIO
		value = "=input.VT_FECHA";
		OBJECT fecha {
			value = "=input.VT_FECHA";
			posx = 0;
			posy = 0;
			sizex = 50;
		}
#endif // AGRUPAR_POR_FECHA_DIARIO
#ifdef AGRUPAR_POR_DOCUMENTO
		value = "=input.DOCUMENTO_GRUPO";
		OBJECT fecha {
			value = "=input.VT_FECHA";
			posx = -1;
			sizex = 6;
			posy = 0;
		}
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = -1;
			sizex = 2;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			sizex = 7;
			posx = -1;
		}
		OBJECT numero {
			value = "=input.VT_NUMERO";
			posx = -1;
			sizex = 12;
			posy = 0;
			adjustment = "reducefont";
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
			sizex = 22;
		}
#endif // AGRUPAR_POR_DOCUMENTO

		OBJECT VT_CANTIDAD_grupo {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 77;
			sizex = 10;
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
			posx = 88;
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

		OBJECT articulo_totales {
			value = "Totales: ";
			posx = 2;
			sizex = 8;
		}
		OBJECT VT_CANTIDAD_total {
			value = "=input.VT_CANTIDAD";
			aggregate = sum;
			posx = 75;
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
			posx = 88;
			sizex = 12;
			alignment = right;
		}
	}