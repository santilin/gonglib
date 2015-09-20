
    DETAILS details {
        posx = 2;
        level = 0;
        style = detail;
        sizey = 1;

        OBJECT DOCUMENTODET_CANTIDAD {
            halignment = "right";
            value = "=input.DOCUMENTODET_CANTIDAD;";
            posx = 0.0;
            sizex = 8.0;
        }
// 		OBJECT ARTICULO_CODIGO {
// 			value = "=input.ARTICULO_CODIGO;";
// 			posx = 9.0;
// 			sizex = 15.0;
// 		}
        OBJECT ARTICULO_NOMBRE {
            value = "= if( @input.ARTICULO_DESCRIPCION == \"\" ) { return @input.ARTICULO_NOMBRE; } else { return @input.ARTICULO_DESCRIPCION; };";
            posx = 9.0;
            sizex = 57.0;
            adjustment = "grow";
        }
        OBJECT DOCUMENTODET_COSTESINIVA {
            formulabefore = "= if( @input.IVADETALLADO != 4 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            halignment = "right";
            value = "=input.DOCUMENTODET_COSTESINIVA;";
            posx = 69.0;
            sizex = 11.0;
        }
        OBJECT DOCUMENTODET_COSTE {
            formulabefore = "= if( @input.IVADETALLADO == 4 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            halignment = "right";
            value = "=input.DOCUMENTODET_COSTE;";
            posx = 69.0;
            sizex = 11.0;
        }
        OBJECT DOCUMENTODET_DTOP100 {
            halignment = "right";
            value = "=input.DOCUMENTODET_DTOP100;";
            format = "%b";
            mask = "999.99";
            posx = 82.0;
            sizex = 6.0;
        }
        OBJECT DOCUMENTODET_IMPORTE {
            formulabefore = "= if( @input.IVADETALLADO != 4 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            halignment = "right";
            value = "=input.DOCUMENTODET_IMPORTE;";
            posx = 88.0;
            sizex = 12.0;
        }
        OBJECT DOCUMENTODET_IMPORTECONIVA {
            formulabefore = "= if( @input.IVADETALLADO == 4 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            halignment = "right";
            value = "=input.DOCUMENTODET_IMPORTECONIVA;";
            posx = 88.0;
            sizex = 12.0;
        }
//>>>>>DETAILS_0
        posx = 0;
    }

    GROUPFOOTER page_footer_notas {
		level = 1;
        sizey = 2;
        style = pagefooter;

        OBJECT pagefooter_DOCUMENTO_NOTAS {
            value = "= input.DOCUMENTO_NOTAS;";
            posy = 0;
            posx = 0.0;
            sizey = 2;
        }
    }
