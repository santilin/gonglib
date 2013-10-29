REPORT Modelo303_Inc
{
	Units = chars;
	SizeX = 80;
	SizeY = 60;
	Tag = "Facturación";
	Title = "###TITULO###";

	PARAMETER IVADETALLADO_CON_RECARGO {
		type = int;
		value = "1";
	}
	PARAMETER asociacion {
		type = text;
		value = "GestiONG";
	}
	PARAMETER seleccion {
		type = text;
		value = "";
	}
	PARAMETER TITULO {
		type = text;
		value = "Modelo 303";
	}

//>>>>>REPORT_DEFINITION


	INPUT default {
		driver = mysql;
#ifdef SEPARARIVAS
		orderby = "T.NOMBRE";

		INPUTFIELD NOMBRE {
			Source = T.NOMBRE;
			Type = string;
		}
		INPUTFIELD IVAP100 {
			Source = T.IVA;
			Type = double;
		}
		INPUTFIELD RECARGOP100 {
			Source = T.RECARGO;
			Type = double;
		}
		INPUTFIELD BASEIVA {
			Source = SUM(FD.IMPORTE);
			Type = money;
		}
		INPUTFIELD BASERECARGO {
			Source = "IF(F.IVADETALLADO=###IVADETALLADO_CON_RECARGO###,SUM(FD.IMPORTE),0)";
			Type = money;
		}
#endif
		INPUTFIELD FACTURAVENTA_FECHAIVA {
			Source = F.FECHAIVA;
			Type = date;
		}
		INPUTFIELD FACTURAVENTA_NUMERO {
			Source = F.NUMERO;
			Type = string;
		}
		INPUTFIELD CLIENTE_RAZONSOCIAL {
			Source = RS.RAZONSOCIAL;
			Type = string;
		}
#ifndef SEPARARIVAS
		orderby = "F.FECHAIVA";

		INPUTFIELD FACTURAVENTA_BASEIMPONIBLE {
			Source = F.BASEIMPONIBLE;
			Type = money;
		}
		INPUTFIELD FACTURAVENTA_IVA {
			Source = F.IVA;
			Type = money;
		}
		INPUTFIELD FACTURAVENTA_TOTAL {
			Source = F.TOTAL;
			Type = money;
		}
		INPUTFIELD FACTURAVENTA_DESGLOSEIVA {
			Source = F.DESGLOSEIVA;
			Type = string;
		}
#endif
	}

	PAGEHEADER page_header_1 {
		sizey = 4;
		style = pageheader;
		backgroundimage = logo;

		OBJECT asociacion {
			style = footer ;
			halignment = left;
			value="###asociacion###" ;
			fontweight = bold;
			sizey = 1;
			sizex = 56;
		}
		OBJECT reportdate {
			style = pagenumber;
			value="= @report.date;";
			format="%d/%m/%Y %H:%M";
			sizey = 1;
			posx = -1 ;
			sizex = 14;
		}
		OBJECT npagina_text  {
			style = pagenumber ;
			halignment = right ;
			value="Pág." ;
			sizey = 1;
			posx = -1;
			sizex = 4 ;
		}
		OBJECT npagina {
			style = pagenumber ;
			sizey = 1;
			posx = -1 ;
			halignment = right ;
			value="= @report.pagenumber;";
			mask = "0000" ;
			sizex = 3;
		}

		OBJECT titulo {
			style = title;
			value = "= @report.title;";
			posy = 2;
			sizey = 1;
			sizex = 80;
			halignment = center;
			fontsize = 12;
			fontweight = bold;
		}
		OBJECT seleccion {
			adjustment = grow;
			halignment = right;
			fontsize = 8;
			posy = 1;
			sizey = 1;
			value="= @report.filter;";
		}
	}

	PAGEHEADER header_captions {
		style = header;
		sizey = 1.1;
		paddingtop = 0.1;
		fontweight = "bold";

		OBJECT header_FACTURAVENTA_FECHAIVA {
			value = "= const @DDD.FACTURAVENTA_FECHAIVA_CAPTION;";
			posx = 0;
			sizex = 6;
		}
		OBJECT header_FACTURAVENTA_NUMERO {
			value = "= const @DDD.FACTURAVENTA_NUMERO_CAPTION;";
			posx = -1;
			sizex = 12;
		}
		OBJECT header_CLIENTE_RAZONSOCIAL {
			value = "= const @DDD.CLIENTE_RAZONSOCIAL_CAPTION;";
			posx = -1;
#ifdef SEPARARIVAS
			sizex = 27;
#endif
#ifndef SEPARARIVAS
			sizex = 14;
#endif
		}
#ifdef SEPARARIVAS
		OBJECT header_BASE {
			value = "= const @DDD.FACTURAVENTA_BASEIMPONIBLE_CAPTION;";
			posx = -1;
			sizex = 11;
			alignment = right;
		}
		OBJECT header_IVA {
			value = "IVA";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT RECARGO {
			value = "RECARGO";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
#endif
#ifndef SEPARARIVAS
		OBJECT header_FACTURAVENTA_BASEIMPONIBLE {
			value = "= const @DDD.FACTURAVENTA_BASEIMPONIBLE_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT header_FACTURAVENTA_IVA {
			value = "= const @DDD.FACTURAVENTA_IVA_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
		OBJECT header_FACTURAVENTA_TOTAL {
			value = "= const @DDD.FACTURAVENTA_TOTAL_CAPTION;";
			posx = -1;
			sizex = 10;
			alignment = right;
		}
#endif
	}

#ifdef SEPARARIVAS
	GROUPHEADER portipoiva {
		level = 1;
		sizey = 1;
		fontweight = "bold";
		value = "=input.NOMBRE";

		OBJECT TIPOIVA {
			value = '= "IVA " + @input.NOMBRE + ": (" + tostring(@input.IVAP100) + "%, " + tostring(@input.RECARGOP100) + "%)";';
			sizey = 1;
			posx = 4;
		}
	}
#endif

	DETAILS details {
		level = 0;
		style = detail;
		sizey = 1;
#ifdef RESUMIDO
		visible = false;
#endif

		OBJECT FACTURAVENTA_FECHAIVA {
			value = "=input.FACTURAVENTA_FECHAIVA";
			halignment = "left";
			posx = 0;
			sizex = 6;
			supressdup = true;
		}
		OBJECT FACTURAVENTA_NUMERO {
			value = "=input.FACTURAVENTA_NUMERO";
			halignment = "left";
			posx = -1;
			sizex = 12;
			supressdup = true;
		}
		OBJECT CLIENTE_RAZONSOCIAL {
			value = "=input.CLIENTE_RAZONSOCIAL";
			halignment = "left";
			posx = -1;
#ifdef SEPARARIVAS
			sizex = 27;
#endif
#ifndef SEPARARIVAS
			sizex = 14;
#endif
			supressdup = true;
		}
#ifdef SEPARARIVAS
		OBJECT BASEIVA {
			value = "=input.BASEIVA";
			posx = -1;
			sizex = 11;
			alignment = right;
		}
		OBJECT IVA {
			value = "= tomoney(@input.BASEIVA * @input.IVAP100 / 100);";
			posx = -1;
			sizex = 10;
			alignment = right;
			format = "%$";
		}
		OBJECT RECARGO {
			value = "= tomoney(@input.BASERECARGO * @input.RECARGOP100 / 100);";
			posx = -1;
			sizex = 10;
			alignment = right;
			format = "%$";
		}
#endif
#ifndef SEPARARIVAS
		OBJECT FACTURAVENTA_BASEIMPONIBLE {
			value = "=input.FACTURAVENTA_BASEIMPONIBLE";
			halignment = "right";
			posx = -1;
			sizex = 10;
		}
		OBJECT FACTURAVENTA_IVA {
			value = "=input.FACTURAVENTA_IVA";
			halignment = "right";
			posx = -1;
			sizex = 10;
		}
		OBJECT FACTURAVENTA_TOTAL {
			value = "=input.FACTURAVENTA_TOTAL";
			halignment = "right";
			posx = -1;
			sizex = 10;
		}
		OBJECT FACTURAVENTA_DESGLOSEIVA {
			value = "=input.FACTURAVENTA_DESGLOSEIVA";
			halignment = "right";
			posx = -1;
			sizex = 10;
		}
#endif
	}

#ifdef SEPARARIVAS
	GROUPFOOTER portipoiva2
	{
		level = 1;
		sizey = 1.2;
		fontweight = bold;
		bordertopstyle = solid;
		margintop = 0.2;
		paddingtop = 0.4;
		value = "=input.NOMBRE";

		OBJECT articulo_totales {
			posy = 0.2;
			value = '= "Totales IVA " + @input.NOMBRE + ": (" + tostring(@input.IVAP100) + "%, " + tostring(@input.RECARGOP100) + "%)";';
			posx = 4;
			sizex = 30;
		}
		OBJECT BASEIVA_TOTAL {
			posy = 0.2;
			value = "=input.BASEIVA";
			aggregate = sum;
			posx = 50;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
		OBJECT IVA_TOTAL {
			posy = 0.2;
			value = "= tomoney(@input.BASEIVA * @input.IVAP100 / 100);";
			aggregate = sum;
			posx = -1;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
		OBJECT RECARGO_total {
			posy = 0.2;
			value = "= tomoney(@input.BASERECARGO * @input.RECARGOP100 / 100);";
			aggregate = sum;
			posx = -1;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
	}
#endif

	REPORTFOOTER report_footer
	{
		sizey = 1.2;
		fontweight = bold;
		bordertopstyle = solid;
		margintop = 0.8;
		paddingtop = 0.4;

		OBJECT articulo_totales {
			posy = 0.2;
			value = "Totales: ";
			posx = 2;
			sizex = 20;
		}
#ifdef SEPARARIVAS
		OBJECT BASEIVA_TOTAL {
			posy = 0.2;
			value = "=input.BASEIVA";
			aggregate = sum;
			posx = 50;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
		OBJECT IVA_TOTAL {
			posy = 0.2;
			value = "= tomoney(@input.BASEIVA * @input.IVAP100 / 100);";
			aggregate = sum;
			posx = -1;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
		OBJECT RECARGO_total {
			posy = 0.2;
			value = "= tomoney(@input.BASERECARGO * @input.RECARGOP100 / 100);";
			aggregate = sum;
			posx = -1;
			sizex = 10;
			format = "%$";
			alignment = right;
		}
#endif
#ifndef SEPARARIVAS
		OBJECT suma_FACTURAVENTA_BASEIMPONIBLE {
			value = "=input.FACTURAVENTA_BASEIMPONIBLE";
			halignment = "right";
			posx = 34;
			sizex = 9;
			aggregate = sum;
		}
		OBJECT suma_FACTURAVENTA_IVA {
			value = "=input.FACTURAVENTA_IVA";
			halignment = "right";
			posx = -1;
			sizex = 10;
			aggregate = sum;
		}
		OBJECT suma_FACTURAVENTA_TOTAL {
			value = "=input.FACTURAVENTA_TOTAL";
			halignment = "right";
			posx = -1;
			sizex = 10;
			aggregate = sum;
		}
#endif
	}