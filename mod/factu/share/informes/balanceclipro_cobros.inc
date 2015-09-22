REPORT BalanceCliPro_Inc
{
	Units = chars;
	SizeX = 80;
	SizeY = 60;
	Tag = "Estadísticas";
	Title = "###TITULO###";
#ifndef AGRUPAR
	SizeX = 120;
#endif

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

#ifdef COBROS
		INPUTFIELD VT_FACTURA_ID {
			Source = VT.FACTURA_ID;
			Type = string;
		}
		INPUTFIELD VT_COBRO_DOCUMENTOPAGO {
			Source = VT.DOCUMENTOPAGO;
			Type = string;
		}
		INPUTFIELD VT_COBRO_FECHAPAGO {
			Source = VT.FECHAPAGO;
			Type = date;
		}
		INPUTFIELD VT_COBRO_IMPORTE {
			Source = VT.COBRO_IMPORTE;
			Type = money;
		}
		INPUTFIELD VT_COBRO_RESTO {
			Source = VT.COBRO_RESTO;
			Type = money;
		}
		INPUTFIELD VT_CUENTAPAGO {
			Source = VT.CUENTA;
			Type = string;
		}
		INPUTFIELD VT_CUENTA_DESCRIPCION {
			Source = VT.CUENTA_DESCRIPCION;
			Type = string;
		}
		INPUTFIELD VT_FORMAPAGO {
			Source = VT.FORMAPAGO;
			Type = string;
		}
#endif // COBROS

	}

	include = "default_pageheader.inc";

	PAGEHEADER page_header_default {
		OBJECT titulo {
#ifndef AGRUPAR
	SizeX = 120;
#endif
		}
	}

#ifdef AGRUPAR_POR_CLIPRO

	GROUPHEADER grupo_clipro {
		level = 1;
		sizey = 1.4;
		fontweight = "bold";
		value = "=input.RAZONSOCIAL_CODIGO";

		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = 2;
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

#endif // AGRUPAR_POR_CLIPRO

#ifdef AGRUPAR_POR_CLIPRO
	GROUPHEADER grupo_clipro_captions
	{
		value = "=input.RAZONSOCIAL_CODIGO";
		level = 1;
#endif
#ifndef AGRUPAR_POR_CLIPRO
	PAGEHEADER grupo_clipro_captions
	{
#endif
		style = header;
		sizey = 1.1;
		fontweight = "bold";
		paddingtop = 0.1;

#ifndef RESUMIDO
		OBJECT VT_FECHA {
			value = "= const @DDD.FACTURAVENTA_FECHA_CAPTION;";
			posx = 0;
			sizex = 6;
		}
		OBJECT VT_TIPODOC {
			value = "= const @DDD.TIPODOC_DESCSINGULAR;";
			posx = 7;
			sizex = 10;
		}
		OBJECT VT_NUMERO {
			value = "= const @DDD.FACTURAVENTA_NUMERO_CAPTION;";
			posx = 18;
			sizex = 8;
		}
#endif // RESUMIDO

		OBJECT VT_BASEIMPONIBLE {
			value = "= const @DDD.FACTURAVENTA_BASEIMPONIBLE_CAPTION;";
			posx = 26;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_IVA {
			value = "= const @DDD.FACTURAVENTA_IVA_CAPTION;";
			posx = 37;
			sizex = 9;
			alignment = right;
		}
		OBJECT VT_TOTAL {
			value = "= const @DDD.FACTURAVENTA_TOTAL_CAPTION;";
			posx = 47;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_PAGADO {
			value = "= const @DDD.FACTURAVENTA_COBROS_CAPTION;";
			posx = 58;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_RESTO {
			value = "= const @DDD.FACTURAVENTA_RESTO_CAPTION;";
			posx = 69;
			sizex = 10;
			alignment = right;
		}
	}

#ifdef AGRUPAR_POR_CLIPRO
	GROUPFOOTER grupo_footer {
		level = 1;
		sizey = 1.2;
		fontweight = "bold";
		value = "=input.VT_FACTURA_ID";

		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = 26;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = 37;
			sizex = 9;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = 47;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = 58;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = 69;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
	}
#endif // AGRUPAR_POR_CLIPRO


	GROUPHEADER grupo_totales_facturas {
		level = 2;
		sizey = 1;
		value = "=input.VT_FACTURA_ID;";
#ifdef RESUMIDO
		visible = false;
#endif

		OBJECT VT_FECHA {
			value = "=input.VT_FECHA";
			posx = 0;
			sizex = 6;
		}
		OBJECT VT_TIPO {
			value = "=input.VT_TIPO";
			posx = 7;
			sizex = 2;
		}
		OBJECT VT_TIPODOC {
			value = "=input.VT_TIPODOC";
			posx = 10;
			sizex = 6;
		}
		OBJECT VT_NUMERO {
			value = "=input.VT_NUMERO";
			posx = 17;
			sizex = 8;
		}
		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = 26;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = 37;
			sizex = 9;
			alignment = right;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = 47;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = 58;
			sizex = 10;
			alignment = right;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = 69;
			sizex = 10;
			alignment = right;
		}
#ifndef AGRUPAR
		OBJECT razonsocial_codigo {
			value = "=input.RAZONSOCIAL_CODIGO";
			posx = 80;
			sizex = 5;
		}
		OBJECT razonsocial_nombre {
			value = "=input.RAZONSOCIAL_NOMBRE";
			posx = 86;
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

	DETAILS details_cobros {
		style = detail;
		sizey = 1;
		supifblank = true;
		fontitalic = true;

		OBJECT VT_FORMAPAGO {
			value = "=input.VT_FORMAPAGO";
			posx = 5;
			sizex = 15;
			alignment = left;
		}
		OBJECT VT_COBRO_DESCRIPCION {
			value = "=input.VT_COBRO_DOCUMENTOPAGO";
			posx = 21;
			sizex = 15;
			alignment = left;
		}
		OBJECT VT_CUENTA_DESCRIPCION {
			value = "=input.VT_CUENTA_DESCRIPCION";
			posx = 37;
			sizex = 15;
			alignment = left;
		}
		OBJECT VT_COBRO_FECHAPAGO {
			value = "=input.VT_COBRO_FECHAPAGO";
			posx = 53;
			sizex = 8;
			alignment = left;
		}
		OBJECT VT_COBRO_IMPORTE {
			value = "=input.VT_COBRO_IMPORTE";
			posx = 62;
			sizex = 10;
			alignment = right;
			format = "%b";
		}
	}

#ifndef RESUMIDO
#ifdef NOTAS
#ifndef CSV
	DETAILS details_notas {
		style = detail;
		sizey = 1;
		supifblank = true;
		fontitalic = true;

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
		value = "=input.VT_FACTURA_ID";

		OBJECT razonsocial_totales {
			value = "Totales: ";
			posx = 2;
			sizex = 9;
		}
		OBJECT VT_BASEIMPONIBLE {
			value = "=input.VT_BASEIMPONIBLE";
			posx = 26;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_IVA {
			value = "=input.VT_IVA";
			posx = 37;
			sizex = 9;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_TOTAL {
			value = "=input.VT_TOTAL";
			posx = 47;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_PAGADO {
			value = "=input.VT_PAGADO";
			posx = 58;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
		OBJECT VT_RESTO {
			value = "=input.VT_RESTO";
			posx = 69;
			sizex = 10;
			alignment = right;
			aggregate = distinctsum;
		}
	}
#endif // GRAN_TOTAL

// El informe se queda abierto
