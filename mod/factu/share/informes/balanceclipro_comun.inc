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
		INPUTFIELD VT_TOTAL {
			Source = VT.TOTAL;
			Type = money;
		}
		INPUTFIELD VT_NOTAS {
			Source = VT.NOTAS;
			Type = text;
		}
		INPUTFIELD RAZONSOCIAL_CODIGO {
			Source = RS.CODIGO;
			Type = integer;
		}
		INPUTFIELD RAZONSOCIAL_NOMBRE {
			Source = RS.RAZONSOCIAL;
			Type = string;
		}
		INPUTFIELD RAZONSOCIAL_ID {
			Source = RS.ID;
			Type = string;
		}
	}

	include = "default_pageheader.inc";

#ifdef AGRUPAR_POR_CLIPRO

	GROUPHEADER grupo_clipro {
		level = 2;
		sizey = 1.4;
		fontweight = "bold";
		fontcolor = "blue";
		fontsize = "12";
		value = "=input.RAZONSOCIAL_CODIGO";

		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = 0;
			posy = 0.2;
			sizex = 5;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL_NOMBRE";
			posx = 6;
			posy = 0.2;
			sizex = 50;
		}
	}
#endif

#ifdef RESUMIDO
	PAGEHEADER header_captions
	{
#endif
#ifndef RESUMIDO
	GROUPHEADER header_captions
	{
#endif
#ifdef AGRUPAR_POR_CLIPRO
		marginleft = 2;
#endif
		style = header;
		level = 2;
		sizey = 1.1;
		fontitalic = "true";
		paddingtop = 0.1;

#ifndef RESUMIDO
		OBJECT VT_FECHA {
			value = "= const @DDD.FACTURAVENTA_FECHA_CAPTION;";
			posx = 0;
			sizex = 8;
		}
		OBJECT VT_TIPODOC {
			value = "= const @DDD.TIPODOC_DESCSINGULAR;";
			posx = 9;
			sizex = 20;
		}
		OBJECT VT_NUMERO {
			value = "= const @DDD.FACTURAVENTA_NUMERO_CAPTION;";
			posx = 30;
			sizex = 12;
		}
		OBJECT VT_TOTAL {
			value = "= const @DDD.FACTURAVENTA_TOTAL_CAPTION;";
			posx = 43;
			sizex = 12;
			alignment = right;
		}
#ifndef AGRUPAR_POR_CLIPRO
		OBJECT VT_CLIENTE {
			value = "= const @DDD.CLIENTE_DESCSINGULAR;";
			posx = -4;
			sizex = 40;
		}
#endif
#endif
	}


#ifdef AGRUPAR_POR_CLIPRO

	GROUPFOOTER grupo_footer {
		level = 2;
		sizey = 1.2;
		fontweight = "bold";

		OBJECT razonsocial_totales {
			value = "Saldo: ";
			posx = 34;
			sizex = 9;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = -1;
			sizex = 14;
			alignment = right;
			aggregate = sum;
		}
	}
#endif

	DETAILS details {
		level = 1;
		style = detail;
		sizey = 1;
#ifdef RESUMIDO
		visible = false;
#endif
#ifdef AGRUPAR_POR_CLIPRO
		marginleft = 2;
#endif

		OBJECT VT_FECHA {
			value = "=input.VT_FECHA";
			posx = 0;
			sizex = 8;
		}
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = 9;
			sizex = 3;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			posx = 13;
			sizex = 16;
		}
		OBJECT VT_NUMERO {
			value = "=input.VT_NUMERO";
			posx = 30;
			sizex = 12;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = 43;
			sizex = 12;
			alignment = right;
		}
#ifndef AGRUPAR
		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = -1; // Ok
			sizex = 5;
			alignment = right;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL_NOMBRE";
			posx = -1; // Ok
		}
#ifdef CSV
#ifdef NOTAS
		OBJECT VT_NOTAS {
			value = "=input.VT_NOTAS";
			posx = -1; // Ok
		}
#endif
#endif
#endif // AGRUPAR

	}

#ifndef RESUMIDO
#ifdef NOTAS
#ifndef CSV
	DETAILS details_notas {
		level = 1;
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
#endif
#endif

#ifdef GRAN_TOTAL
	REPORTFOOTER report_footer
	{
		sizey = 1.2;
		fontweight = bold;
		bordertopstyle = solid;
		paddingtop = 0.6;

		OBJECT razonsocial_totales {
			value = "Saldo total: ";
			posx = 26;
			sizex = 15;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = -1; // Ok
			sizex = 14;
			alignment = right;
			aggregate = sum;
		}
	}
#endif

// El informe se queda abierto
