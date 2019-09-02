	PAGEHEADER page_header {
		sizey = 8;
		style = pageheader;

		OBJECT pageheader_EMPRESA_LOGO {
			value = "= input.EMPRESA_LOGO;";
			posy = 0.0;
			posx = 0.0;
			sizey = 5;
			sizex = 20;
			imagestyle = "expand";
		}
#ifdef INCNOMBREEMPRESA
 		OBJECT pageheader_EMPRESA_NOMBRE {
 			value = "=input.EMPRESA_CONTACTO_NOMBRE;";
 			posy = 1.0;
			sizey = 1.0;
 			posx = 20.0;
 			fontsize = "10";
 			fontweight = bold;
 		}
#endif

		OBJECT pageheader_EMPRESA_CIF {
			value = "=input.EMPRESA_CONTACTO_CIF;";
			posy = 1.0;
			posx = 0.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_DIRECCION {
			value = "=input.EMPRESA_CONTACTO_DIRECCION;";
			posy = 2.0;
			posx = 0.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_LOCALIDAD {
			value = "=input.EMPRESA_CONTACTO_LOCALIDAD;";
			posy = 3.0;
			posx = 0.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_CP {
			value = "=input.EMPRESA_CONTACTO_CP;";
			posy = 4.0;
			posx = 0.0;
			sizex = 7.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_PROVINCIA {
			value = "=input.EMPRESA_CONTACTO_PROVINCIA;";
			posy = 4.0;
			posx = 8.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_TELEFONO {
			value = "=input.EMPRESA_CONTACTO_TELEFONO;";
			posy = 5.0;
			posx = 0.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_EMAIL {
			value = "=input.EMPRESA_CONTACTO_EMAIL;";
			posy = 6.0;
			posx = 0.0;
			sizex = 20.0;
			adjustment = reducefont;
			minfontsize = 4;
			fontweight = bold;
		}
	}
