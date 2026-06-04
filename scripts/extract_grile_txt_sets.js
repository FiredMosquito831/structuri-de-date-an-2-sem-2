const fs = require('fs');
const path = require('path');

const SOURCES = [
  {
    id: 'grile1',
    file: 'C:\\Users\\fgghk\\Downloads\\SDD_grile_FINAL_verified_outputs\\SDD_grile_FINAL_verified\\outputs_txt_docx\\grile1.txt',
  },
  {
    id: 'grile2',
    file: 'C:\\Users\\fgghk\\Downloads\\SDD_grile_FINAL_verified_outputs\\SDD_grile_FINAL_verified\\outputs_txt_docx\\grile2.txt',
  },
  {
    id: 'grile3',
    file: 'C:\\Users\\fgghk\\Downloads\\SDD_grile_FINAL_verified_outputs\\SDD_grile_FINAL_verified\\outputs_txt_docx\\grile3.txt',
  },
  {
    id: 'grile4',
    file: 'C:\\Users\\fgghk\\Downloads\\SDD_grile_FINAL_verified_outputs\\SDD_grile_FINAL_verified\\outputs_txt_docx\\grile4.txt',
  },
];

const siteRoot = path.resolve(__dirname, '..');

function normalizeText(text) {
  return String(text || '')
    .replace(/\r\n/g, '\n')
    .replace(/\r/g, '\n')
    .replace(/[ \t]+$/gm, '')
    .trim();
}

function splitQuestionBlocks(text) {
  const normalized = normalizeText(text);
  const matches = [...normalized.matchAll(/^Question\s+(\d+)\s*$/gmi)];
  if (!matches.length) {
    throw new Error('No "Question N" blocks found.');
  }

  return matches.map((match, index) => {
    const start = match.index + match[0].length;
    const end = index + 1 < matches.length ? matches[index + 1].index : normalized.length;
    return {
      sourceQuestionNumber: Number(match[1]),
      body: normalized.slice(start, end).trim(),
    };
  });
}

function parseCorrectLine(lines, sourceQuestionNumber) {
  const correctIndex = lines.findIndex(line => /^Correct\s*:/i.test(line.trim()));
  if (correctIndex === -1) {
    throw new Error(`Question ${sourceQuestionNumber}: missing Correct line.`);
  }

  const correctLine = lines[correctIndex].trim();
  const correctNumbers = [...correctLine.matchAll(/\d+/g)].map(match => Number(match[0]));
  if (!correctNumbers.length) {
    throw new Error(`Question ${sourceQuestionNumber}: Correct line has no numeric option indices.`);
  }

  return {
    correctIndex,
    correctNumbers: new Set(correctNumbers),
  };
}

function parseOptions(lines, sourceQuestionNumber) {
  const firstOptionIndex = lines.findIndex(line => /^\s*\d+\.\s+/.test(line));
  if (firstOptionIndex === -1) {
    throw new Error(`Question ${sourceQuestionNumber}: no numbered options found.`);
  }

  const questionLines = lines.slice(0, firstOptionIndex);
  const optionLines = lines.slice(firstOptionIndex);
  const options = [];

  for (const line of optionLines) {
    const optionMatch = line.match(/^\s*(\d+)\.\s*(.*)$/);
    if (optionMatch) {
      options.push({
        number: Number(optionMatch[1]),
        textLines: [optionMatch[2].trim()],
      });
      continue;
    }

    if (!options.length) {
      throw new Error(`Question ${sourceQuestionNumber}: option continuation before first option.`);
    }

    // Preserve multi-line options if a future source line wraps.
    options[options.length - 1].textLines.push(line.trim());
  }

  const answers = options.map(option => ({
    number: option.number,
    text: option.textLines.filter(Boolean).join('\n').trim(),
  }));

  if (answers.some(answer => !answer.text)) {
    throw new Error(`Question ${sourceQuestionNumber}: blank option text.`);
  }

  return {
    questionText: questionLines.join('\n').trim(),
    answers,
  };
}

function parseQuestionBlock(block, index, sourceId) {
  const lines = block.body.split('\n').map(line => line.trimEnd());
  const { correctIndex, correctNumbers } = parseCorrectLine(lines, block.sourceQuestionNumber);
  const usefulLines = lines.slice(0, correctIndex).filter((line, lineIndex, allLines) => {
    if (line.trim()) return true;
    const prev = allLines[lineIndex - 1] || '';
    const next = allLines[lineIndex + 1] || '';
    return prev.trim() && next.trim();
  });

  const { questionText, answers } = parseOptions(usefulLines, block.sourceQuestionNumber);
  const optionNumbers = new Set(answers.map(answer => answer.number));
  const missingCorrects = [...correctNumbers].filter(number => !optionNumbers.has(number));
  if (missingCorrects.length) {
    throw new Error(`Question ${block.sourceQuestionNumber}: Correct option(s) not present: ${missingCorrects.join(', ')}`);
  }

  return {
    id: index + 1,
    question: questionText,
    source: `${sourceId}.txt`,
    sourceQuestionNumber: block.sourceQuestionNumber,
    answers: answers.map(answer => ({
      text: answer.text,
      isCorrect: correctNumbers.has(answer.number),
    })),
  };
}

function validate(questions, sourceId) {
  if (!questions.length) {
    throw new Error(`${sourceId}: extractor produced no questions.`);
  }

  const errors = [];
  questions.forEach((question, index) => {
    if (!question.question) errors.push(`${sourceId} #${index + 1}: missing question text`);
    if (!Array.isArray(question.answers) || !question.answers.length) errors.push(`${sourceId} #${index + 1}: missing answers`);
    if (question.answers.some(answer => !answer.text)) errors.push(`${sourceId} #${index + 1}: blank answer text`);
    if (!question.answers.some(answer => answer.isCorrect)) errors.push(`${sourceId} #${index + 1}: no correct answer`);
  });

  if (errors.length) {
    throw new Error(errors.join('\n'));
  }
}

function writeSet(source) {
  const rawText = fs.readFileSync(source.file, 'utf8');
  const blocks = splitQuestionBlocks(rawText);
  const questions = blocks.map((block, index) => parseQuestionBlock(block, index, source.id));
  validate(questions, source.id);

  const json = `${JSON.stringify(questions, null, 2)}\n`;
  const roPath = path.join(siteRoot, 'sets', `${source.id}.json`);
  const enPath = path.join(siteRoot, 'sets_en', `${source.id}.json`);
  fs.writeFileSync(roPath, json, 'utf8');
  fs.writeFileSync(enPath, json, 'utf8');

  const multiCorrect = questions.filter(question => question.answers.filter(answer => answer.isCorrect).length > 1).length;
  return {
    id: source.id,
    count: questions.length,
    singleCorrect: questions.length - multiCorrect,
    multiCorrect,
    roPath,
    enPath,
  };
}

const results = SOURCES.map(writeSet);
for (const result of results) {
  console.log(`${result.id}: ${result.count} questions (${result.singleCorrect} single-correct, ${result.multiCorrect} multi-correct)`);
  console.log(`  wrote ${result.roPath}`);
  console.log(`  wrote ${result.enPath}`);
}
