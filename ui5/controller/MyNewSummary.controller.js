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
"rootui5/eve7/controller/Ged.controller",
"sap/m/CustomTreeItem",
"sap/ui/core/HTML"
   ], function(SummaryController, EveManager, JSONModel, Table, Column, Text,ColumnListItem, Sorter, XMLView, SplitterLayoutData, GedController, CustomTreeItem, HTML) {
"use strict";

// Tree item that shows the table of a collection's items. It has an elementId
// property because the base Summary controller calls getElementId() on every
// tree item when it handles highlight and selection.
let SumTableItem = CustomTreeItem.extend("custom.SumTableItem", {
   renderer: {},
   metadata: { properties: { elementId: "string" } }
});

// Compact table in the style of the Fireworks summary view.
const kSumTableCss = `
.vsdSumTable { font-size: 0.7rem; margin: 0.1rem 0 0.3rem 0; max-width: 100%; overflow-x: auto; }
.vsdSumTable table { border-collapse: collapse; }
.vsdSumTable th { font-weight: normal; background: var(--sapList_HeaderBackground, #e5e5e5); color: var(--sapList_HeaderTextColor, inherit); padding: 0 0.3rem; text-align: center; }
.vsdSumTable td { border: 1px solid var(--sapList_BorderColor, #c8c8c8); color: var(--sapTextColor, inherit); padding: 0 0.3rem; text-align: right; white-space: nowrap; font-variant-numeric: tabular-nums; }
.vsdSumTable td.vsdRowHead { text-align: left; padding-left: 0.15rem; }
.vsdSumTable input { margin: 0 0.2rem 0 0; vertical-align: middle; width: 0.8rem; height: 0.8rem; }
.vsdSumTable .vsdSwatch { display: inline-block; width: 0.6rem; height: 0.6rem; margin-right: 0.25rem; vertical-align: middle; border: 1px solid #888; }
.vsdSumTable tr.vsdFiltered td { opacity: 0.5; }
.vsdSumTable tbody tr { cursor: pointer; }
.vsdSumTable tr.vsdSelected td { background: rgb(66, 124, 172); color: #fff; }
.vsdSumTableItem .sapMLIBContent { min-width: 0; }
/* the table needs no tree indentation; the tree sets it as an inline padding */
.vsdSumTableItem.sapMTreeItemBase { padding-left: 0.4rem !important; }
.vsdSumTableItem .sapMTreeItemBaseExpander { display: none; }
/* The edit button of a summary row is shorter than the icon's line height,
   which puts the pencil on its bottom edge. Center the icon instead. */
.eveSummaryItem .sapMBtn .sapMBtnInner { display: flex; align-items: center; justify-content: center; }
.eveSummaryItem .sapMBtn .sapMBtnIcon { line-height: 1; }
/* a little room between a row's checkbox and its name; the FlexBox wraps
   each of them in a sapMFlexItem */
.eveSummaryItem .sapMFlexItem:has(> .sapMCb) + .sapMFlexItem { margin-left: 0.25rem; }
`;

function escapeHtml(s) {
   return String(s).replace(/[&<>"]/g, c => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[c]));
}

return SummaryController.extend("custom.MyNewSummary", {

onInit: function() {
this.showOnlyCollectons = true;
SummaryController.prototype.onInit.apply(this, arguments);
this.expandLevel = 0;

if (!document.getElementById("vsdSumTableCss")) {
   let style = document.createElement("style");
   style.id = "vsdSumTableCss";
   style.textContent = kSumTableCss;
   document.head.appendChild(style);
}
this.bindTreeItems();
},

event: function(lst) {
SummaryController.prototype.event(lst);
oTree.expandToLevel(0);
},

/** Bind the tree with a factory: collection tables get their own item type. */
bindTreeItems: function() {
   let oTree = this.getView().byId("tree");
   oTree.unbindItems();
   oTree.bindItems({ path: "treeModel>/", factory: this.treeItemFactory.bind(this) });
},

treeItemFactory: function(sId, oContext) {
   if (!oContext.getProperty("fSumTable"))
      return this.template.clone(sId);

   let item = new SumTableItem(sId, {
      elementId: "{treeModel>fElementId}",
      content: [ new HTML({ content: "{treeModel>fSumTableHtml}", sanitizeContent: false }) ]
   });
   item.addStyleClass("vsdSumTableItem");
   item.attachBrowserEvent("click", this.onSumTableClick, this);
   return item;
},

/** HTML for the table of a collection: first three Table-view columns, from the
  * collection's sumTable, with a visibility checkbox, colour and name per item. */
makeSumTableHtml: function(coll) {
   let st = coll.sumTable;
   let itemList = coll.childs ? coll.childs.find(c => c.items) : null;
   let items = itemList ? itemList.items : [];

   // item name from the collection name: Jets -> Jet, KsVertices -> KsVertex
   let name = coll.fName;
   if (name.endsWith("ices"))
      name = name.slice(0, -4) + "ex";
   else if (name.length > 1 && name.endsWith("s"))
      name = name.slice(0, -1);

   let selected = this.selectedIndices(itemList);

   let html = '<div class="vsdSumTable"><table><thead><tr><th></th>';
   for (let c of st.cols)
      html += "<th>" + escapeHtml(c) + "</th>";
   html += "</tr></thead><tbody>";

   st.rows.forEach((row, i) => {
      let it = items[i] || {};
      let color = EVE.JSR.getColor(it.fColor !== undefined ? it.fColor : coll.fMainColor);
      let cls = [it.fFiltered ? "vsdFiltered" : "", selected.has(i) ? "vsdSelected" : ""].join(" ").trim();
      html += '<tr data-idx="' + i + '"' + (cls ? ' class="' + cls + '"' : '') + '><td class="vsdRowHead">' +
              '<input type="checkbox" data-idx="' + i + '"' + (it.fRnrSelf !== false ? ' checked' : '') + '>' +
              '<span class="vsdSwatch" style="background:' + color + '"></span>' +
              escapeHtml(name + " " + i) + "</td>";
      for (let v of row)
         html += "<td>" + escapeHtml(v) + "</td>";
      html += "</tr>";
   });

   if (!st.rows.length)
      html += '<tr><td class="vsdRowHead" colspan="' + (st.cols.length + 1) + '">no items</td></tr>';

   return html + "</tbody></table></div>";
},

/** Indices of the items of a collection's item list in the global selection,
  * as recorded from SelectElement/UnselectElement. The selection's own sel_list
  * cannot be used later: EveManager empties it once the change is dispatched. */
selectedIndices: function(itemList) {
   return (itemList && this.sumSelected && this.sumSelected[itemList.fElementId]) || new Set();
},

/** A row checkbox toggles the item's visibility, as in the collection editor;
  * a click anywhere else in a row selects the item (Ctrl adds to the selection),
  * as a row selection in the Table view does. */
onSumTableClick: function(oEvent) {
   let target = oEvent.target;
   if (!target)
      return;
   let isCheckbox = target.tagName === "INPUT" && target.dataset.idx !== undefined;
   let row = isCheckbox ? null : target.closest("tbody tr[data-idx]");
   if (!isCheckbox && !row)
      return;

   let item = sap.ui.getCore().byId(oEvent.currentTarget.id);
   let collId = item && item.getBindingContext("treeModel").getProperty("collectionId");
   let coll = collId !== undefined ? this.mgr.GetElement(collId) : null;
   // SetItemVisible is a method of the collection's item list, not of the collection
   let itemList = coll && coll.childs ? coll.childs.find(c => c.items) : null;
   if (!itemList)
      return;

   if (isCheckbox) {
      this.mgr.SendMIR("SetItemVisible(" + target.dataset.idx + ", " + target.checked + ")", itemList.fElementId, itemList._typename);
      return;
   }

   let multi = oEvent.ctrlKey || oEvent.metaKey ? true : false;
   this.mgr.SendMIR("ProcessSelectionStr(" + this.mgr.global_selection_id + ", " + multi + ", true, \" " + row.dataset.idx + " \")",
                    itemList.fElementId, itemList._typename);
},

createModel: function() {
this.sumTableNodes = {};          // collection id -> its table node
this.itemListToCollection = {};   // item list id -> collection id

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

/** As in the base Summary controller, except that a collection with a sumTable
  * gets one child that shows its items as a table, instead of its item list. */
createSummaryModel: function(tgt, src, path) {
   for (let n = 0; n < src.length; ++n) {
      let elem = src[n];

      // exclude members of EveWorld (fElementId === 1) like Camera list
      if (elem.fMotherId === 1 && elem.fName === "Cameras")
         continue;

      let newelem = { fName: elem.fName, fTitle: elem.fTitle || elem.fName, id: elem.fElementId, fHighlight: "None", fBackground: "", fMainColor: "", fSelected: false };

      this.setElementsAttributes(newelem, elem);

      newelem.path = path + n;
      newelem.masterid = elem.fMasterId || elem.fElementId;

      tgt.push(newelem);

      this.summaryElements[newelem.id] = newelem;

      if (elem.sumTable) {
         let tnode = { fSumTable: true, fName: "", collectionId: elem.fElementId,
                       path: newelem.path + "/childs/0", fSumTableHtml: this.makeSumTableHtml(elem) };
         newelem.childs = [ tnode ];
         this.sumTableNodes[elem.fElementId] = tnode;
         for (let c of (elem.childs || []))
            this.itemListToCollection[c.fElementId] = elem.fElementId;
      } else if ((elem.childs !== undefined) && this.anyVisible(elem.childs)) {
         newelem.childs = this.createSummaryModel([], elem.childs, newelem.path + "/childs/");
      }
   }

   return tgt;
},

/** Refresh a collection's table in place when the collection or its item list
  * changes, so expanded collections stay expanded. */
sceneElementChange: function(msg) {
   SummaryController.prototype.sceneElementChange.apply(this, arguments);

   let collId = this.sumTableNodes && this.sumTableNodes[msg.fElementId] ? msg.fElementId
                                                                          : (this.itemListToCollection || {})[msg.fElementId];
   let tnode = collId !== undefined ? this.sumTableNodes[collId] : null;
   let coll = tnode ? this.mgr.GetElement(collId) : null;
   if (coll && coll.sumTable) {
      tnode.fSumTableHtml = this.makeSumTableHtml(coll);
      this.any_changed = true;
   }
},

/** Selection changed: record the selected items of a collection's item list and
  * refresh its table. */
SelectElement: function(selection_obj, element_id, sec_idcs) {
   SummaryController.prototype.SelectElement.apply(this, arguments);
   if (selection_obj && selection_obj.fElementId == this.mgr.global_selection_id) {
      this.sumSelected = this.sumSelected || {};
      this.sumSelected[element_id] = new Set((sec_idcs || []).map(Number));
      this.refreshSelectionInTable(element_id);
   }
},

UnselectElement: function(selection_obj, element_id) {
   SummaryController.prototype.UnselectElement.apply(this, arguments);
   if (selection_obj && selection_obj.fElementId == this.mgr.global_selection_id && this.sumSelected) {
      delete this.sumSelected[element_id];
      this.refreshSelectionInTable(element_id);
   }
},

refreshSelectionInTable: function(element_id) {
   let collId = (this.itemListToCollection || {})[element_id];
   let tnode = collId !== undefined ? this.sumTableNodes[collId] : null;
   let coll = tnode ? this.mgr.GetElement(collId) : null;
   if (coll && coll.sumTable) {
      tnode.fSumTableHtml = this.makeSumTableHtml(coll);
      this.getView().getModel("treeModel").refresh();
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

// The base class rebuilds the tree with a fixed template; rebuild with the factory instead.
// A rebuild happens on every event (the event's elements are removed), so keep the
// expanded top-level entries, e.g. collections whose table is open, expanded.
if (this.rebuild) {
   let oTree = this.getView().byId("tree");
   let expandedIds = new Set();
   for (let item of oTree.getItems())
      if (item.getExpanded() && item.getLevel() === 0)
         expandedIds.add(item.getBindingContext("treeModel").getProperty("id"));

   let model = this.getView().getModel("treeModel");
   model.setData(this.createModel());
   model.refresh();
   this.bindTreeItems();
   oTree.expandToLevel(this.expandLevel);

   // one call with all indices: expanding one at a time lets the tree re-render in between
   let indices = [];
   oTree.getItems().forEach((item, k) => {
      if (item.getLevel() === 0 && expandedIds.has(item.getBindingContext("treeModel").getProperty("id")))
         indices.push(k);
   });
   if (indices.length)
      oTree.expand(indices);

   if (this.ged_visible)
      this.ged.getController().buildEditor();

   this.rebuild = false;
   return;
}
SummaryController.prototype.endChanges.apply(this, arguments);
},
});
});
