// Get the page (counter value, not real page number) for a selector
// e. g. #page_ref(<lst:hello-world>)
#let page_ref(selector) = {
  locate(loc => {
    // Get the `location` of the element
    let element_location = query(selector, loc)
    .first()
    .location()

    // Get the page number, the location lies on
    link(element_location)[#counter(page).at(element_location).first()]
  })
}
