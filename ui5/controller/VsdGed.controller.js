sap.ui.define(['rootui5/eve7/controller/Ged.controller',
"sap/ui/core/mvc/XMLView"
], function(GedController, XMLView) {
   "use strict";

   return GedController.extend("custom.VsdGed", {

      onInit: function() {
         GedController.prototype.onInit.apply(this, arguments);

         let oModel = new sap.ui.model.json.JSONModel();
         this.getView().setModel(oModel, "edm")
        },


     // event item
     buildFWDataCollectionSetter : function (el) {
         this.buildREveDataCollectionSetter(el);
         this.oModel.setProperty("/title", this.editorElement.fName + " Editor");
         if (el.var) {
         el.var.forEach((par) => {
             let fname = "makePBC" + par.type + "Setter";
             this[fname](par);
         });
         }
     },

    // bool setter
   makePBCBoolSetter :  function(par)
    {
        let gedFrame = this.getView().byId("GED");
        let gcm = this;
        let widget = new sap.m.CheckBox({
        selected: par.val,
            
        select: function (oEvent) {
            console.log("Bool setter select event", oEvent.getSource());
            let funcName = "UpdatePBParameter";
            let value = oEvent.getSource().getSelected();
            let mir = funcName + "( \"" + oEvent.getSource().desc + "\",\"" + value + "\" )";
            gcm.mgr.SendMIR(mir, gcm.editorElement.fElementId, gcm.editorElement._typename);
        }
        });

        widget.desc = par.name;
            
        let label = new sap.m.Text({ text: par.name });
        label.addStyleClass("sapUiTinyMargin");
            
        let frame = new sap.ui.layout.HorizontalLayout({
        content: [widget, label]
        });
            
        gedFrame.addContent(frame);
    },

    makePBCLongSetter : function(par)
    {
        let gedFrame = this.getView().byId("GED");
        let gcm = this;
        let widget = new sap.m.StepInput({
        min: par.min,
        max: par.max,
        value: par.val,
        change: function (oEvent) {
            console.log("Long setter select event", oEvent.getSource());
            let funcName = "UpdatePBParameter";
            let mir = funcName + "( \"" + oEvent.getSource().desc + "\",\"" + oEvent.getParameter("value") + "\" )";
            gcm.mgr.SendMIR(mir, gcm.editorElement.fElementId, gcm.editorElement._typename);
        }
        });

     widget.desc = par.name;
        
     let label = new sap.m.Text({ text: par.name });
     label.addStyleClass("sapUiTinyMargin");
        
     let frame = new sap.ui.layout.HorizontalLayout({
     content: [widget, label]
     });
        
     gedFrame.addContent(frame);
    },
 
    // ROOT color index, edited with a palette that also opens a full color picker.
    // The server sends the color as "rgb" and takes the choice back as "r,g,b".
    makePBCColorSetter : function(par)
    {
        let gcm = this;
        let widget = new sap.m.Button({
            icon: "sap-icon://palette",
            tooltip: "Color " + par.val,
            press: function () {
                let button = this;
                let popover = new sap.m.ColorPalettePopover({
                    colors: ['rgb(204,204,204)', 'rgb(153,153,153)', 'rgb(102,102,102)', 'black', 'white',
                             'red', 'rgb(200,0,200)', 'blue', 'cyan', 'lime',
                             'gold', 'darkorange', 'indianred', 'rgb(102,51,0)', 'slategray'],
                    defaultColor: par.rgb,
                    showMoreColorsButton: true,
                    colorSelect: function (oEvent) {
                        // the palette gives names, hex or rgb(); let the browser normalize it
                        let probe = document.createElement("div");
                        probe.style.color = oEvent.getParameter("value");
                        document.body.appendChild(probe);
                        let rgb = getComputedStyle(probe).color.match(/\d+/g);
                        document.body.removeChild(probe);
                        if (!rgb) return;
                        let mir = "UpdatePBParameter( \"" + par.name + "\",\"" + rgb.slice(0, 3).join(",") + "\" )";
                        gcm.mgr.SendMIR(mir, gcm.editorElement.fElementId, gcm.editorElement._typename);
                    }
                });
                popover.openBy(button);
            }
        });
        // show the current color on the button
        widget.addEventDelegate({
            onAfterRendering: function () { widget.$().children().css("background-color", par.rgb); }
        });

        let label = new sap.m.Text({ text: par.name });
        label.addStyleClass("sapUiTinyMargin");

        let frame = new sap.ui.layout.HorizontalLayout({
            content: [widget, label]
        });

        let gedFrame = this.getView().byId("GED");
        gedFrame.addContent(frame);
    },

    makePBCDoubleSetter : function(par)
    {  
     let gcm = this;
     let widget = new sap.m.Input({
     value: par.val,
     width: "5em", // room for about 4 characters
     tooltip: "limit value [ " + par.min + ", " + par.max + "]",
     change: function (oEvent) {
         console.log("Long setter select event", oEvent.getSource());
         
         // validation
         let ival = oEvent.getParameter("value");
         
         if (ival > par.max || ival < par.min) {
             let iControl = oEvent.getSource();
             iControl.setValueState(sap.ui.core.ValueState.Error);
         }
         else {
             oEvent.getSource().setValueState(sap.ui.core.ValueState.None);
             let funcName = "UpdatePBParameter";
             let mir = funcName + "( \"" + oEvent.getSource().desc + "\",\"" + oEvent.getParameter("value") + "\" )";
             gcm.mgr.SendMIR(mir, gcm.editorElement.fElementId, gcm.editorElement._typename);
         }
     }
     });

     widget.desc = par.name;
        
     let label = new sap.m.Text({ text: par.name });
     label.addStyleClass("sapUiTinyMargin");
        
     let frame = new sap.ui.layout.HorizontalLayout({
     content: [widget, label]
     });
        
     let gedFrame = this.getView().byId("GED");
     gedFrame.addContent(frame);
    }


    });
}); 
