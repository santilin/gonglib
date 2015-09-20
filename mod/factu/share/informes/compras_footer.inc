	GROUPFOOTER page_footer_values {
		level = 1;
		posy = -3;
        sizey = 2.5;
        style = pagefooter;

        OBJECT pagefooter_values_DOCUMENTO_SUMAIMPORTES_SIN_IVA {
            formulabefore = "= if( @input.IVADETALLADO != 4) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= input.DOCUMENTO_SUMAIMPORTES;";
            posx = 0.0;
            sizex = 16.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_SUMAIMPORTES_CON_IVA {
            formulabefore = "= if( @input.IVADETALLADO == 4 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= return @input.DOCUMENTO_TOTAL - @input.DOCUMENTO_DESCUENTO;";
            posx = 0.0;
            sizex = 16.0;
            format = "%$";
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_DTOP100_ABRE {
            formulabefore = "= if( @input.DOCUMENTO_DTOP100 == 0.0 ) {@this.visible = false;} else { @this.visible = true;} return true;";
            value = "(";
            posx = 19.0;
            sizex = 2.0;
        }
        OBJECT pagefooter_values_DOCUMENTO_DTOP100 {
            value = "= input.DOCUMENTO_DTOP100;";
            format = "%b";
            mask = "999.99";
            posx = 20.0;
            sizex = 5.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_DTOP100_CIERRA {
            formulabefore = "= if( @input.DOCUMENTO_DTOP100 == 0.0 ) {@this.visible = false;} else { @this.visible = true;} return true;";
            value = "%)";
            posx = 25.0;
            sizex = 4.0;
        }
        OBJECT pagefooter_values_DOCUMENTO_DESCUENTO {
            value = "= input.DOCUMENTO_DESCUENTO;";
            format = "%b";
            mask = "999.99";
            posx = 28.0;
            sizex = 10.0;
            halignment = "left";
        }
        // Sin recargo de equivalencia
        OBJECT pagefooter_values_DOCUMENTO_BASEIMPONIBLE_SIN_RECARGO {
            formulabefore = "= if( @input.IVADETALLADO != 2 && @input.IVADETALLADO != 4) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= input.DOCUMENTO_BASEIMPONIBLE;";
            posx = 55.0;
            sizex = 14.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_IVA_SIN_RECARGO {
            formulabefore = "= if( @input.IVADETALLADO != 2 && @input.IVADETALLADO != 4) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= @input.DOCUMENTO_IVA";
            posx = 70.0;
            sizex = 14.0;
            halignment = "right";
        }
        // Con recargo de equivalencia
        OBJECT pagefooter_values_DOCUMENTO_BASEIMPONIBLE_CON_RECARGO {
            formulabefore = "= if( @input.IVADETALLADO == 2 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= input.DOCUMENTO_BASEIMPONIBLE;";
            posx = 40.0;
            sizex = 14.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_IVA_CON_RECARGO {
            formulabefore = "= if( @input.IVADETALLADO == 2 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= input.DOCUMENTO_IVA;";
            posx = 55.0;
            sizex = 14.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_RECARGOEQUIVALENCIA {
            formulabefore = "= if( @input.IVADETALLADO == 2 ) { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= input.DOCUMENTO_RECARGOEQUIVALENCIA;";
            posx = 70.0;
            sizex = 12.0;
            halignment = "right";
        }
        OBJECT pagefooter_values_DOCUMENTO_TOTAL {
            value = "= input.DOCUMENTO_TOTAL;";
            posx = 85.0;
            sizex = 15.0;
            halignment = "right";
            fontweight = "bold";
        }
        OBJECT pagefooter_values_DOCUMENTO_DESGLOSEIVA {
            formulabefore = "= if( @input.IVADETALLADO != 3 && @input.IVADETALLADO != 4 && @input.DESGLOSEIVA != \"\") { @this.visible = true; } else { @this.visible = false; } return true;";
            value = "= return \"Desglose del IVA: \" + @input.DESGLOSEIVA";
			posy = 1.2;
            posx = 10.0;
            sizex = 74.0;
			fontitalic = true;
            halignment = "right";
        }
        
		pageAfter = true;
    }
