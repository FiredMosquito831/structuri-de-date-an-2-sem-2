# Structuri de Date - An 2 Sem 2 MCQ Practice

Static multiple-choice quiz website for Data Structures practice.

The site is built for browser-only use: no backend, no database, no build step. It can run locally from a simple static server and can be deployed directly with GitHub Pages.

## Contents

- `index.html`: main quiz application.
- `answers.html`: searchable correct-answer viewer.
- `materials.html`: placeholder page for future course materials.
- `materials/all_in_one_courses.pdf`: complete course PDF for MCQ theory preparation.
- `materials/complete_cheatsheet.c`: C code and theory cheatsheet for Test 1 and Test 2 implementation patterns.
- `data/config.json`: central set registry, display order, default set, labels, and file paths.
- `sets/`: Romanian/current-language MCQ JSON sets.
- `sets_en/`: English-mode MCQ JSON sets. For now these mirror the Romanian/current sets where no translation exists.
- `scripts/extract_test_sdd_html.js`: extractor used to regenerate the SDD test set from the original HTML source.
- `scripts/extract_from_forms.js`: extractor/normalizer for the Google Forms-derived `fromForms` set.
- `scripts/extract_grile_txt_sets.js`: extractor for the corrected verified `grile1`-`grile4` text exports.
- `assets/fromForms/`: local copies of images embedded in the Google Form questions.

## Current Question Sets

The default active set is `sdd_test`, extracted from:

```text
C:\Users\fgghk\Downloads\sdd\test_SDD.html
```

Current counts:

- `sdd_test`: 74 questions
- `fromForms`: 74 questions, including 14 questions with images
- `grile1`: 47 questions
- `grile2`: 44 questions
- `grile3`: 44 questions
- `grile4`: 47 questions
- `arrays`: 3 placeholder questions
- `linked_lists`: 3 placeholder questions
- `trees`: 3 placeholder questions

`sdd_test` contains 62 single-correct questions and 12 multi-correct questions.
`fromForms` is extracted from a public Google Form. The public form does not expose its answer key, so correctness is maintained in `scripts/extract_from_forms.js` using a reviewed answer map and local image assets.
`grile1`-`grile4` are extracted from corrected verified TXT/DOCX outputs and preserve both single-correct and multi-correct answer keys.

## Local Run

From the repository root:

```powershell
python -m http.server 8000
```

Then open:

```text
http://localhost:8000/
```

Opening `index.html` directly from disk may fail in some browsers because the app loads JSON files with `fetch()`.

## Study Materials

The materials page is available at:

```text
materials.html
```

Available downloads:

- `All In One Courses`: PDF with all course content, intended as the main theory reference for MCQ preparation.
- `Complete Cheatsheet C`: C file with code plus theory explanations for applying implementation patterns in Test 1 and Test 2.
- `MCQ JSON Question Sets`: direct downloads for every active quiz set plus `data/config.json`, useful for backup, review, migration, or rebuilding the site.

## Regenerate The SDD Set

If the source HTML changes, regenerate the active set with:

```powershell
node scripts/extract_test_sdd_html.js
```

The script writes both:

```text
sets/sdd_test.json
sets_en/sdd_test.json
```

It also validates that every extracted question has text, answers, and at least one correct answer.

## Regenerate The Google Forms Set

The `fromForms` set is generated with:

```powershell
node scripts/extract_from_forms.js
```

Important details:

- The script reads the public Google Forms model from the form URL.
- Question images are referenced from local files under `assets/fromForms/`.
- Correct answers are not exposed by the public Google Form, so they are maintained in the extractor's `CORRECT_BY_INDEX` map.
- If the Google Form changes, inspect and update the answer map before trusting regenerated output.

## Regenerate The Corrected Grile TXT Sets

The corrected verified grile sets are generated from:

```text
C:\Users\fgghk\Downloads\SDD_grile_FINAL_verified_outputs\SDD_grile_FINAL_verified\outputs_txt_docx\grile1.txt
C:\Users\fgghk\Downloads\SDD_grile_FINAL_verified_outputs\SDD_grile_FINAL_verified\outputs_txt_docx\grile2.txt
C:\Users\fgghk\Downloads\SDD_grile_FINAL_verified_outputs\SDD_grile_FINAL_verified\outputs_txt_docx\grile3.txt
C:\Users\fgghk\Downloads\SDD_grile_FINAL_verified_outputs\SDD_grile_FINAL_verified\outputs_txt_docx\grile4.txt
```

Regenerate them with:

```powershell
node scripts/extract_grile_txt_sets.js
```

The script writes both Romanian/current and English-mode copies:

```text
sets/grile1.json
sets/grile2.json
sets/grile3.json
sets/grile4.json
sets_en/grile1.json
sets_en/grile2.json
sets_en/grile3.json
sets_en/grile4.json
```

The parser expects blocks shaped as `Question N`, prompt/code text, numbered options, then `Correct: ...`. It validates that every correct option index exists and every question has at least one correct answer.

## Add A New Question Set

1. Add the JSON file under `sets/`.
2. Add an English version or copy under `sets_en/`.
3. Register the set in `data/config.json`.
4. Add the set ID to `displayOrder`.
5. Set `defaultSet` if it should be selected by default.
6. Update `APP_DATA_VERSION` in `index.html` and `answers.html` so browsers do not keep stale cached data.
7. Validate before committing.

Preferred JSON format:

```json
[
  {
    "id": 1,
    "question": "Question text?",
    "images": [
      { "src": "assets/fromForms/q21_01.png", "alt": "Question image" }
    ],
    "answers": [
      { "text": "Option A", "isCorrect": false },
      {
        "text": "Option B",
        "isCorrect": true,
        "images": [
          { "src": "assets/fromForms/q35_04.png", "alt": "Option image" }
        ]
      }
    ]
  }
]
```

## Validation

Run this from the repository root before pushing:

```powershell
@'
import json
from pathlib import Path

root = Path(".")
cfg = json.loads((root / "data/config.json").read_text(encoding="utf-8"))

assert cfg["defaultSet"] in cfg["sets"]
for set_id in cfg["displayOrder"]:
    meta = cfg["sets"][set_id]
    for key in ["dataFile", "englishDataFile"]:
        path = root / meta[key]
        data = json.loads(path.read_text(encoding="utf-8"))
        assert isinstance(data, list), path
        for i, q in enumerate(data, 1):
            assert q.get("question"), (path, i, "missing question")
            assert q.get("answers"), (path, i, "missing answers")
            assert any(a.get("isCorrect") for a in q["answers"]), (path, i, "no correct answer")

print("OK")
'@ | python -
```

## Deployment

This repository is ready for GitHub Pages branch deployment.

Recommended Pages settings:

- Source: deploy from branch
- Branch: `main`
- Folder: `/ (root)`

After pushing, the expected Pages URL is:

```text
https://firedmosquito831.github.io/structuri-de-date-an-2-sem-2/
```

If the online site still shows old data, hard refresh the browser after the Pages build finishes.
