type item = {
  id: int,
  title: string,
  completed: bool
};

let str = ReasonReact.stringToElement;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, ~onToggle, _children) => {
    ...component,
    render: (_self) => {
      let className = 
      ["todo", item.completed === true ? "completed" : ""]
      |>  String.concat(" ");
      <li className>
        <div className="view">
          <input
            className="toggle"
            _type="checkbox"
            onClick=((_evt) => onToggle())
            checked=(Js.Boolean.to_js_boolean(item.completed))
          />
          <label> (str(item.title)) </label>
          <button className="destroy" />
        </div>
      </li>  
    }
  };
};

let valueFromEvent = (evt) : string => (
                                         evt
                                         |> ReactEventRe.Form.target
                                         |> ReactDOMRe.domElementToObj
                                       )##value;

module Input = {
  type state = string;
  let component = ReasonReact.reducerComponent("Input");
  let make = (~onSubmit, _) => {
    ...component,
    initialState: () => "",
    reducer: (newText, _text) => ReasonReact.Update(newText),
    render: ({state: text, reduce}) =>
      <input
        className="new-todo"
        value=text
        _type="text"
        placeholder="What needs to be done"
        onChange=(reduce((evt) => valueFromEvent(evt)))
        onKeyDown=(
          (evt) =>
            if (ReactEventRe.Keyboard.key(evt) == "Enter") {
              onSubmit(text);
              (reduce(() => ""))()
            }
        )
      />
  };
};

type state = {items: list(item)};

type action =
  | AddItem(string)
  | ToggleItem(int);

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = ref(0);

let newItem = (text) => {
  lastId := lastId^ + 1;
  {id: lastId^, title: text, completed: false}
};

let make = (_children) => {
  ...component,
  initialState: () => {items: [{id: 0, title: "Hello from ReasonReact", completed: false}]},
  reducer: (action, {items}) =>
    switch action {
    | AddItem(text) => ReasonReact.Update({items: [newItem(text), ...items]})
    | ToggleItem(id) =>
      let items =
        List.map((item) => item.id === id ? {...item, completed: ! item.completed} : item, items);
      ReasonReact.Update({items: items})
    },
  render: ({state: {items}, reduce}) => {
    let incompleteItemsCount = List.length(List.filter((item) => item.completed === false, items));
    let getIncompleteItemsStr = (completedItemsCount) =>
      str(completedItemsCount === 1 ? " item left" : " items left");
    <section className="todoapp">
      <header className="header">
        <h1> (str("todos")) </h1>
        <Input onSubmit=(reduce((text) => AddItem(text))) />
      </header>
      <section className="main">
        <ul className="todo-list">
          (
            ReasonReact.arrayToElement(
              Array.of_list(
                List.map(
                  (item) =>
                    <TodoItem
                      key=(string_of_int(item.id))
                      onToggle=(reduce(() => ToggleItem(item.id)))
                      item
                    />,
                  items
                )
              )
            )
          )
        </ul>
      </section>
      <div className="footer">
        <span className="todo-count">
          <strong> (str(string_of_int(incompleteItemsCount))) </strong>
          (getIncompleteItemsStr(incompleteItemsCount))
        </span>
      </div>
    </section>
  }
};