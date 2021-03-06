	GROUPHEADER page_header {
		level = 1;
		value = "=input.DOCUMENTO_ID";
		sizey = 11.5;
		style = pageheader;

		OBJECT pageheader_EMPRESA_LOGO {
			value = "= input.EMPRESA_LOGO;";
			posy = 0.0;
			posx = 0.0;
			sizey = 4;
			sizex = 40;
			imagestyle = "expanded";
		}
// 		OBJECT pageheader_EMPRESA_NOMBRE {
// 			value = "=input.EMPRESA_CONTACTO_NOMBRE;";
// 			posy = 2.0;
// 			posx = 5.0;
// 			fontsize = "14";
// 			fontweight = bold;
// 		}
		OBJECT pageheader_EMPRESA_CIF_CAPTION {
			value = "= const @DDD.CONTACTO_CIF_CAPTION;";
			posy = 5.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_CIF {
			value = "=input.EMPRESA_CONTACTO_CIF;";
			posy = 5.0;
			posx = 13.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_CONTACTO_DIRECCION_CAPTION {
			value = "= const @DDD.CONTACTO_DIRECCION_CAPTION;";
			posy = 6.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_DIRECCION {
			value = "=input.EMPRESA_CONTACTO_DIRECCION;";
			posy = 6.0;
			posx = 13.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_LOCALIDAD_CAPTION {
			value = "= const @DDD.CONTACTO_LOCALIDAD_CAPTION;";
			posy = 7.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_LOCALIDAD {
			value = "=input.EMPRESA_CONTACTO_LOCALIDAD;";
			posy = 7.0;
			posx = 13.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_PROVINCIA_CAPTION {
			value = "= const @DDD.CONTACTO_PROVINCIA_CAPTION;";
			posy = 8.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_CP {
			value = "=input.EMPRESA_CONTACTO_CP;";
			posy = 8.0;
			posx = 13.0;
			sizex = 7.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_PROVINCIA {
			value = "=input.EMPRESA_CONTACTO_PROVINCIA;";
			posy = 8.0;
			posx = 21.0;
			sizex = 14.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_TELEFONO_CAPTION {
			value = "= const @DDD.CONTACTO_TELEFONO_CAPTION;";
			posy = 9.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_TELEFONO {
			value = "=input.EMPRESA_CONTACTO_TELEFONO;";
			posy = 9.0;
			posx = 13.0;
			fontweight = bold;
		}
		OBJECT pageheader_EMPRESA_EMAIL_CAPTION {
			value = "= const @DDD.CONTACTO_EMAIL_CAPTION;";
			posy = 10.0;
			posx = 0.0;
			sizex = 12.0;
		}
		OBJECT pageheader_EMPRESA_EMAIL {
			value = "=input.EMPRESA_CONTACTO_EMAIL;";
			posy = 10.0;
			posx = 13.0;
			fontweight = bold;
		}

		OBJECT pageheader_PROVEEDORA_CONTACTO_NOMBRE {
			value = "=input.PROVEEDORA_CONTACTO_NOMBRE;";
			posy = 0.2;
			sizey = 1.6;
			posx = 50.0;
			adjustment = reducefont;
			fontweight = bold;
			fontsize = "16";
		}
		OBJECT pageheader_PROVEEDORA_CIF_CAPTION {
			value = "= const @DDD.CONTACTO_CIF_CAPTION;";
			posy = 2.0;
			posx = 50.0;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_CIF {
			value = "=input.PROVEEDORA_CONTACTO_CIF;";
			posy = 2.0;
			posx = 65.0;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_DIRECCION_CAPTION {
			value = "= const @DDD.CONTACTO_DIRECCION_CAPTION;";
			posy = 3.0;
			posx = 50.0;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_DIRECCION {
			value = "=input.PROVEEDORA_CONTACTO_DIRECCION;";
			posy = 3.0;
			posx = 65.0;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_LOCALIDAD_CAPTION {
			value = "= const @DDD.CONTACTO_LOCALIDAD_CAPTION;";
			posy = 4.0;
			posx = 50;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_LOCALIDAD {
			value = "=input.PROVEEDORA_CONTACTO_LOCALIDAD;";
			posy = 4.0;
			posx = 65.0;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_CP_CAPTION {
			value = "= const @DDD.CONTACTO_CP_CAPTION;";
			posy = 5.0;
			posx = 50;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_CP {
			value = "=input.PROVEEDORA_CONTACTO_CP;";
			posy = 5.0;
			posx = 65.0;
			sizex = 7;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_PROVINCIA {
			value = "=input.PROVEEDORA_CONTACTO_PROVINCIA;";
			posy = 5.0;
			posx = -1; // Ok
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_TELEFONO_CAPTION {
			value = "= const @DDD.CONTACTO_TELEFONO_CAPTION;";
			posy = 6.0;
			posx = 50.0;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_TELEFONO {
			value = "=input.PROVEEDORA_CONTACTO_TELEFONO;";
			posy = 6.0;
			posx = 65.0;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_FAX_CAPTION {
			value = "= const @DDD.CONTACTO_FAX_CAPTION;";
			posy = 7.0;
			posx = 50.0;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_FAX {
			value = "=input.PROVEEDORA_CONTACTO_FAX;";
			posy = 7.0;
			posx = 65.0;
			fontweight = bold;
		}
		OBJECT pageheader_PROVEEDORA_EMAIL_CAPTION {
			value = "= const @DDD.CONTACTO_EMAIL_CAPTION;";
			posy = 8.0;
			posx = 50.0;
			sizex = 12.0;
		}
		OBJECT pageheader_PROVEEDORA_EMAIL {
			value = "=input.PROVEEDORA_CONTACTO_EMAIL;";
			posy = 8.0;
			posx = 65.0;
			fontweight = bold;
		}
	}
