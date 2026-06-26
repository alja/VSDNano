sap.ui.define(['rootui5/eve7/controller/Summary.controller',
	'rootui5/eve7/lib/EveManager',
	"sap/ui/model/json/JSONModel",
	"sap/m/Table",
	"sap/m/Column",
	"sap/m/Text",
"sap/m/ColumnListItem",
"sap/ui/model/Sorter",
"sap/ui/core/mvc/XMLView",
"sap/ui/layout/SplitterLayoutData",
"rootui5/eve7/controller/Ged.controller"
   ], function(SummaryController, EveManager, JSONModel, Table, Column, Text,ColumnListItem, Sorter, XMLView, SplitterLayoutData, GedController) {
"use strict";

return SummaryController.extend("custom.MyNewSummary", {

onInit: function() {
this.showOnlyCollectons = true;
SummaryController.prototype.onInit.apply(this, arguments);
this.expandLevel = 0;
},

event: function(lst) {
SummaryController.prototype.event(lst);
oTree.expandToLevel(0);
},

createModel: function() {
if (this.showOnlyCollectons === true) {
// this is central method now to create summary model
// one could select top main element which will be shown in SummaryView

this.summaryElements = {};

var src = this.mgr.childs[0].childs[2].childs;

for (var i = 0; i < src.length; i++) {
 if (src[i].fName == "Collections")
	src = src[i].childs;
}

return this.createSummaryModel([], src, "/");
}
else {
this.summaryElements = {};
  var src =  this.mgr.childs[0].childs[2].childs;
return this.createSummaryModel([], src, "/");
}
},

showGedEditor: function (elementId) {

var sumSplitter = this.byId("sumSplitter");

if (!this.ged) {
var pthis = this;
XMLView.create({
	viewName: "custom.view.VsdGed",
	layoutData: new SplitterLayoutData("sld", { size: "30%" }),
	height: "100%"
}).then(function (oView) {
	pthis.ged = oView;
	pthis.ged.getController().setManager(pthis.mgr);
	pthis.ged.getController().showGedEditor(sumSplitter, elementId);
});
} else {
this.ged.getController().showGedEditor(sumSplitter, elementId);
}
},

switchMode: function (evt) {

this.showOnlyCollectons = !this.showOnlyCollectons;
if (this.showOnlyCollectons == true)
	this.byId("eveMode").setText("Show Scenes List");
else
	this.byId("eveMode").setText("Show Collections List");
this.rebuild = true;
this.endChanges();
},

endChanges: function () {
sap.ui.getCore().byId("__xmlview0").getController().showEventInfo();
SummaryController.prototype.endChanges.apply(this, arguments);
},
});
});