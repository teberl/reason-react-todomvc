let component = ReasonReact.statelessComponent("TodoFooter");

let str = ReasonReact.stringToElement;

let make = (~incompleteCount, ~completeCount, ~onClearCompletedItems, _children) => {
  ...component,
  render: (_self) => {
    let getIncompleteItemsStr = (completedItemsCount) =>
      str(completedItemsCount === 1 ? " item left" : " items left");
    <div className="footer">
      <span className="todo-count">
        <strong> (str(string_of_int(incompleteCount))) </strong>
        (getIncompleteItemsStr(incompleteCount))
      </span>
      <ul className="filters">
        <li> <a> (str("All")) </a> </li>
        <li> <a> (str("Active")) </a> </li>
        <li> <a> (str("Completed")) </a> </li>
      </ul>
      (
        completeCount > 0 ?
          <button className="clear-completed" onClick=onClearCompletedItems>
            (str("Clear completed"))
          </button> :
          ReasonReact.nullElement
      )
    </div>
  }
};