const fs = require('fs');
const path = require('path');
const vm = require('vm');

const FORM_URL = 'https://docs.google.com/forms/d/1HseVAii-eQXPU09E_hgqCxTXlrWrrUH9hkxjPsyCJ0o/viewform?edit_requested=true';
const SOURCE_HTML = path.join(__dirname, 'fromForms_source.html');
const ROOT = path.resolve(__dirname, '..');
const OUT_RO = path.join(ROOT, 'sets', 'fromForms.json');
const OUT_EN = path.join(ROOT, 'sets_en', 'fromForms.json');

const CORRECT_BY_INDEX = {
  1: [2],
  2: [1, 5],
  3: [5],
  4: [3, 5],
  5: [2],
  6: [4],
  7: [1, 2],
  8: [4],
  9: [2],
  10: [1, 5],
  11: [4],
  12: [2, 4, 5],
  13: [5],
  14: [4],
  15: [3, 4],
  16: [1, 2, 3, 4],
  17: [2, 5],
  18: [2],
  19: [4],
  20: [1],
  21: [1],
  22: [1],
  23: [1],
  24: [3],
  25: [4],
  26: [1, 2, 3, 4],
  27: [1, 2, 3],
  28: [3, 4],
  29: [1],
  30: [1],
  31: [1, 2, 4],
  32: [2],
  33: [1, 2, 3, 4],
  34: [2, 3],
  35: [3, 5],
  36: [3],
  37: [1],
  38: [2],
  39: [1],
  40: [5],
  41: [1, 2],
  42: [1],
  43: [4],
  44: [3, 4],
  45: [1],
  46: [4, 5],
  47: [2, 4],
  48: [1],
  49: [1],
  50: [3],
  51: [4, 5],
  52: [3],
  53: [2, 3],
  54: [1, 2, 3, 4],
  55: [1],
  56: [4],
  57: [3],
  58: [1],
  59: [3],
  60: [4],
  61: [2],
  62: [5],
  63: [2],
  64: [3],
  65: [2],
  66: [4],
  67: [2],
  68: [1, 4, 5],
  69: [4, 5],
  70: [4],
  71: [1],
  72: [1, 2],
  73: [2],
  74: [2, 5],
};

function extractArrayLiteral(html) {
  const marker = 'var FB_PUBLIC_LOAD_DATA_ = ';
  const start = html.indexOf(marker);
  if (start < 0) throw new Error('FB_PUBLIC_LOAD_DATA_ not found');
  const arrayStart = html.indexOf('[', start);
  let depth = 0;
  let quote = null;
  let escaped = false;
  for (let i = arrayStart; i < html.length; i++) {
    const ch = html[i];
    if (quote) {
      if (escaped) escaped = false;
      else if (ch === '\\') escaped = true;
      else if (ch === quote) quote = null;
      continue;
    }
    if (ch === '"' || ch === "'" || ch === '`') {
      quote = ch;
      continue;
    }
    if (ch === '[') depth++;
    if (ch === ']') {
      depth--;
      if (depth === 0) return html.slice(arrayStart, i + 1);
    }
  }
  throw new Error('FB_PUBLIC_LOAD_DATA_ array end not found');
}

function splitQuestionBlocks(html) {
  return html.split('<div class="Qr7Oae" role="listitem">').slice(1);
}

function extractImageSources(block) {
  return [...block.matchAll(/<img\b[^>]*\bsrc="([^"]+)"/g)].map(match =>
    match[1].replace(/&amp;/g, '&')
  );
}

function cleanText(text) {
  return String(text || '')
    .replace(/\s+/g, ' ')
    .replace(/^[a-zA-Z]\.\s*/, '')
    .replace(/^\d+\.\s*/, '')
    .trim();
}

function localImage(questionIndex, imageIndex) {
  return `assets/fromForms/q${String(questionIndex).padStart(2, '0')}_${String(imageIndex).padStart(2, '0')}.png`;
}

async function loadHtml() {
  if (fs.existsSync(SOURCE_HTML)) return fs.readFileSync(SOURCE_HTML, 'utf8');
  const response = await fetch(FORM_URL, {
    headers: { 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)' },
  });
  if (!response.ok) throw new Error(`Failed to fetch form: ${response.status}`);
  return response.text();
}

function convert(html) {
  const data = vm.runInNewContext(`(${extractArrayLiteral(html)})`);
  const items = data?.[1]?.[1] || [];
  const blocks = splitQuestionBlocks(html);
  if (items.length !== blocks.length) {
    throw new Error(`Model/rendered block mismatch: ${items.length} items vs ${blocks.length} blocks`);
  }

  return items.map((item, itemIndex) => {
    const questionIndex = itemIndex + 1;
    const correct = new Set(CORRECT_BY_INDEX[questionIndex] || []);
    const optionRows = item?.[4]?.[0]?.[1] || [];
    const questionImageCount = item?.[9]?.length || 0;
    const blockImages = extractImageSources(blocks[itemIndex]);
    const optionImageStart = questionImageCount;

    const answers = optionRows.map((option, optionIndex) => {
      const optionImage = option?.[5] ? localImage(questionIndex, optionImageStart + optionIndex + 1) : null;
      return {
        text: cleanText(option?.[0]) || `Option ${optionIndex + 1}`,
        isCorrect: correct.has(optionIndex + 1),
        ...(optionImage ? { images: [{ src: optionImage, alt: `Question ${questionIndex} option ${optionIndex + 1}` }] } : {}),
      };
    });

    const images = Array.from({ length: questionImageCount }, (_, imageIndex) => ({
      src: localImage(questionIndex, imageIndex + 1),
      alt: `Question ${questionIndex} image ${imageIndex + 1}`,
    }));

    return {
      id: questionIndex,
      question: cleanText(item?.[1]),
      source: 'Google Forms',
      sourceId: item?.[0],
      images,
      answers,
    };
  });
}

function validate(questions) {
  const errors = [];
  questions.forEach(question => {
    if (!question.question) errors.push(`Question ${question.id}: missing text`);
    if (!question.answers.length) errors.push(`Question ${question.id}: missing answers`);
    if (!question.answers.some(answer => answer.isCorrect)) errors.push(`Question ${question.id}: no correct answer`);
    for (const image of question.images || []) {
      if (!fs.existsSync(path.join(ROOT, image.src))) errors.push(`Question ${question.id}: missing image ${image.src}`);
    }
    question.answers.forEach((answer, answerIndex) => {
      for (const image of answer.images || []) {
        if (!fs.existsSync(path.join(ROOT, image.src))) errors.push(`Question ${question.id} answer ${answerIndex + 1}: missing image ${image.src}`);
      }
    });
  });
  if (errors.length) throw new Error(errors.join('\n'));
}

loadHtml().then(html => {
  const questions = convert(html);
  validate(questions);
  const json = `${JSON.stringify(questions, null, 2)}\n`;
  fs.writeFileSync(OUT_RO, json, 'utf8');
  fs.writeFileSync(OUT_EN, json, 'utf8');
  const imageQuestions = questions.filter(q => q.images?.length || q.answers.some(a => a.images?.length)).length;
  const multiCorrect = questions.filter(q => q.answers.filter(a => a.isCorrect).length > 1).length;
  console.log(`Extracted ${questions.length} questions`);
  console.log(`Image questions: ${imageQuestions}`);
  console.log(`Multi-correct questions: ${multiCorrect}`);
  console.log(`Wrote ${OUT_RO}`);
  console.log(`Wrote ${OUT_EN}`);
}).catch(error => {
  console.error(error);
  process.exit(1);
});
