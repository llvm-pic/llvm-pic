#import "utils.typ": page_ref
#import "config.typ": conf
#show: doc => conf(
  title: "Bringing PIC Microcontroller Developement into the 21st Century",
  authors: (
    (
      name: "",
      company: "",
      matriculation_number: "",
      department: "",
      course: "",
      course_administrator: "",
      scientific_advisor: "",
      period: "",
      submission_date: "",
    ),
  ),
  university: "Junivercity",
  abstract: [],
  language: "en",
  font: "IBM Plex Serif",
  outlines: (
    ("List of figures", image),
    ("List of tables", image),
    ("List of listings", raw)
  ),
  thesis_type: "",
  course_of_studies: "",
  field_of_studies: "",
  company_logo: (path: "pictures/Company-logo.png", alternative_text: "Company Logo"),
  university_logo: (path:"pictures/University-logo.png", alternative_text: "University Logo"),
  declaration_on_honour: true,
  bibliography-file: "references.yml",
  bibliography-style: "ieee",
  doc,
)

= This is a chapter headline

