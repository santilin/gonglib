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
		INPUTFIELD VT_BASEIMPONIBLE {
			Source = VT.BASEIMPONIBLE;
			Type = money;
		}
		INPUTFIELD VT_IVA {
			Source = VT.IVA;
			Type = money;
		}
		INPUTFIELD VT_TOTAL {
			Source = VT.TOTAL;
			Type = money;
		}
		INPUTFIELD VT_RESTO {
			Source = VT.RESTO;
			Type = money;
		}
		INPUTFIELD VT_SUMAIMPORTES {
			Source = VT.SUMAIMPORTES;
			Type = money;
		}
		INPUTFIELD VT_PAGADO {
			Source = VT.PAGADO;
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
			posx = -1;
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
			sizex = 6;
		}
		OBJECT VT_TIPODOC {
			value = "= const @DDD.TIPODOC_DESCSINGULAR;";
			posx = -1;
			sizex = 10;
		}
		OBJECT VT_NUMERO {
			value = "= const @DDD.FACTURAVENTA_NUMERO_CAPTION;";
			posx = -1;
			sizex = 8;
		}
#endif
		OBJECT VT_BASEIMPONIBLE {
			value = "= const @DDD.FACTURAVENTA_BASEIMPONIBLE_CAPTION;";
			posx = 26;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_IVA {
			value = "= const @DDD.FACTURAVENTA_IVA_CAPTION;";
			posx = -1;
			sizex = 9;
			alignment = right;
		}
		OBJECT VT_TOTAL {
			value = "= const @DDD.FACTURAVENTA_TOTAL_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_PAGADO {
			value = "= const @DDD.FACTURAVENTA_COBROS_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_RESTO {
			value = "= const @DDD.FACTURAVENTA_RESTO_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
	}


#ifdef AGRUPAR_POR_CLIPRO

	GROUPFOOTER grupo_footer {
		level = 2;
		sizey = 1.2;
		fontweight = "bold";

		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = 28;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = -1;
			sizex = 9;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = -1;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = -1;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = -1;
			sizex = 10;
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
			sizex = 6;
		}
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = -1;
			sizex = 2;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			posx = -1;
			sizex = 6;
		}
		OBJECT VT_NUMERO {
			value = "=input.VT_NUMERO";
			posx = -1;
			sizex = 8;
		}
		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = -1;
			sizex = 9;
			alignment = right;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
#ifndef AGRUPAR
		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = -1;
			sizex = 5;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL_NOMBRE";
			posx = -1;
		}
#ifdef CSV
#ifdef NOTAS
		OBJECT VT_NOTAS {
			value = "=input.VT_NOTAS";
			posx = -1;
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
			value = "Totales: ";
			posx = 2;
			sizex = 8;
		}
		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = 28;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = -1;
			sizex = 9;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = -1;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = -1;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = -1;
			sizex = 10;
			alignment = right;
			aggregate = sum;
		}
	}
#endif

// El informe se queda abierto
