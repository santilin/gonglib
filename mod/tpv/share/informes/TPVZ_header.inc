	REPORTHEADER page_header {
		sizey = 11;
		style = pageheader;
		fontfamily = "Verdana";

		OBJECT pageheader_EMPRESA_LOGO {
			margintop = "-0.5";
			value = "= input.EMPRESA_LOGO;";
			posy = 0.0;
			posx = 0.0;
			sizey = 8;
			sizex = 40;
			imagestyle = "center";
		}

		OBJECT pageheader_EMPRESA_CIF_CAPTION {
			value = "= const @DDD.CONTACTO_CIF_CAPTION + \":\";";
			posy = 7.0;
			posx = 0.0;
			sizex = 4.0;
		}
		OBJECT pageheader_EMPRESA_CIF {
			value = "=input.EMPRESA_CONTACTO_CIF;";
			posy = 7.0;
			posx = -1;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_DIRECCION {
			value = "=input.EMPRESA_CONTACTO_DIRECCION;";
			posy = 8.0;
			posx = 0.0;
		}
		OBJECT pageheader_EMPRESA_CP {
			value = "=input.EMPRESA_CONTACTO_CP";
			posy = 8.0;
			posx = 25.0;
		}
		OBJECT pageheader_EMPRESA_LOCALIDAD {
			value = "=input.EMPRESA_CONTACTO_LOCALIDAD";
			posy = 8.0;
			posx = 32.0;
		}
		OBJECT pageheader_EMPRESA_TELEFONO_caption {
			value = "Tel:";
			posy = 9.0;
			posx = 0.0;
		}
		OBJECT pageheader_EMPRESA_TELEFONO {
			value = "=input.EMPRESA_CONTACTO_TELEFONO;";
			posy = 9.0;
			posx = 4.0;
		}
		OBJECT pageheader_EMPRESA_EMAIL {
			value = "=input.EMPRESA_CONTACTO_EMAIL;";
			posy = 10.0;
			posx = 0.0;
		}
	}
