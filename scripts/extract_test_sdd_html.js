const fs = require('fs');
const path = require('path');
const vm = require('vm');

const DEFAULT_SOURCE = 'C:\\Users\\fgghk\\Downloads\\sdd\\test_SDD.html';
const sourcePath = process.argv[2] || DEFAULT_SOURCE;
const siteRoot = path.resolve(__dirname, '..');
const outRo = path.join(siteRoot, 'sets', 'sdd_test.json');
const outEn = path.join(siteRoot, 'sets_en', 'sdd_test.json');

function extractPoolLiteral(html) {
  const startToken = 'const POOL =';
  const start = html.indexOf(startToken);
  if (start === -1) {
    throw new Error('Could not find "const POOL =" in source HTML.');
  }

  const arrayStart = html.indexOf('[', start);
  if (arrayStart === -1) {
    throw new Error('Could not find POOL array start.');
  }

  let depth = 0;
  let quote = null;
  let escaped = false;

  for (let i = arrayStart; i < html.length; i++) {
    const ch = html[i];

    if (quote) {
      if (escaped) {
        escaped = false;
      } else if (ch === '\\') {
        escaped = true;
      } else if (ch === quote) {
        quote = null;
      }
      continue;
    }

    if (ch === '"' || ch === "'" || ch === '`') {
      quote = ch;
      continue;
    }

    if (ch === '[') depth++;
    if (ch === ']') {
      depth--;
      if (depth === 0) {
        return html.slice(arrayStart, i + 1);
      }
    }
  }

  throw new Error('Could not find POOL array end.');
}

function normalizeQuestion(raw, index) {
  const questionParts = [String(raw.q || '').trim()];
  if (raw.code) {
    questionParts.push(`Cod:\n${String(raw.code).trim()}`);
  }

  const correct = new Set(Array.isArray(raw.correct) ? raw.correct : []);
  const answers = (raw.options || []).map((text, optionIndex) => ({
    text: String(text || '').trim(),
    isCorrect: correct.has(optionIndex),
  })).filter(answer => answer.text.length > 0);

  return {
    id: index + 1,
    question: questionParts.filter(Boolean).join('\n\n'),
    category: raw.cat || null,
    sourceType: raw.type || null,
    code: raw.code || undefined,
    answers,
  };
}

function validate(questions) {
  if (!Array.isArray(questions) || questions.length === 0) {
    throw new Error('Extractor produced no questions.');
  }

  const errors = [];
  questions.forEach((question, index) => {
    if (!question.question) errors.push(`Question ${index + 1}: missing text`);
    if (!Array.isArray(question.answers) || question.answers.length === 0) errors.push(`Question ${index + 1}: missing answers`);
    if (question.answers.some(answer => !answer.text)) errors.push(`Question ${index + 1}: blank answer`);
    if (!question.answers.some(answer => answer.isCorrect)) errors.push(`Question ${index + 1}: no correct answer`);
  });

  if (errors.length) {
    throw new Error(errors.join('\n'));
  }
}

const html = fs.readFileSync(sourcePath, 'utf8');
const poolLiteral = extractPoolLiteral(html);
const pool = vm.runInNewContext(`(${poolLiteral})`, Object.create(null), { timeout: 1000 });
const questions = pool.map(normalizeQuestion);
validate(questions);

const json = `${JSON.stringify(questions, null, 2)}\n`;
fs.writeFileSync(outRo, json, 'utf8');
fs.writeFileSync(outEn, json, 'utf8');

const multiCount = questions.filter(question => question.answers.filter(answer => answer.isCorrect).length > 1).length;
const categoryCounts = questions.reduce((counts, question) => {
  const key = question.category || 'Uncategorized';
  counts[key] = (counts[key] || 0) + 1;
  return counts;
}, {});

console.log(`Extracted ${questions.length} questions from ${sourcePath}`);
console.log(`Single-correct: ${questions.length - multiCount}`);
console.log(`Multi-correct: ${multiCount}`);
console.log(`Wrote ${outRo}`);
console.log(`Wrote ${outEn}`);
console.log(JSON.stringify(categoryCounts, null, 2));
