	PARAMETER LOGO {
		type = image;
	}
	PARAMETER SUBTITLE {
		type = text;
		value = "GestiONG";
	}
	PARAMETER SELECCION {
		type = text;
		value = "= @report.filter;";
	}

	PAGEHEADER page_header_default {
		sizey = 4;
		style = pageheader;
		borderstyle = solid;

		OBJECT pageheader_EMPRESA_LOGO {
			value = "###LOGO###";
			posy = 0.0;
			posx = 0.0;
			sizey = 4;
			sizex = 40;
			imagestyle = "expanded";
		}

		OBJECT subtitle {
			halignment = left;
			value="###SUBTITLE###" ;
			fontweight = bold;
			posx = 0;
			posy = 0;
			sizey = 1;
			sizex = 56;
		}
		OBJECT reportdate {
			style = pagenumber;
			value="= const @report.date;";
			format="%d/%m/%Y %H:%M";
			sizey = 1;
			posy = 0;
			posx = 75;
			sizex = 15;
		}
		OBJECT npagina_text  {
			style = pagenumber ;
			value=" Pág." ;
			posy = 0;
			sizey = 1;
			posx = 91;
			sizex = 5;
		}
		OBJECT npagina {
			style = pagenumber ;
			sizey = 1;
			posx = 96; // Ok
			posy = 0;
			value="= @report.pagenumber;";
			mask = "0000" ;
			sizex = 4;
		}

		OBJECT titulo {
			style = title;
			value = "= const @report.title;";
			posx = 0;
			posy = 1.1;
			sizey = 1.2;
			halignment = center;
			fontsize = 13;
			fontweight = bold;
			fontcolor = "blue";
		}
	}

	PAGEHEADER page_header_default_selection {
		sizey = 1;
		style = pageheader;
		backcolor = "red";
		supifblank = true;

		OBJECT seleccion {
			adjustment = grow;
			halignment = right;
			fontsize = 8;
			posy = 3.2;
			sizey = 1;
//			value="= const @report.filter;";
			value="###SELECCION###";
		}
	}
