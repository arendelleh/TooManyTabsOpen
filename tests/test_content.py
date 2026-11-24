import copy
import importlib.util
import json
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("tasks", ROOT / "tools/generate_tasks.py")
tasks = importlib.util.module_from_spec(spec)
spec.loader.exec_module(tasks)


class ContentTests(unittest.TestCase):
    def setUp(self):
        self.data = json.loads((ROOT / "assets/tasks.json").read_text(encoding="utf-8"))

    def test_real_content(self):
        self.assertEqual(len(tasks.validate(self.data)), 4)

    def test_missing_step_is_rejected(self):
        self.data["tasks"][0]["steps"].pop()
        with self.assertRaisesRegex(ValueError, "three"):
            tasks.validate(self.data)

    def test_duplicate_id_is_rejected(self):
        self.data["tasks"][1]["id"] = self.data["tasks"][0]["id"]
        with self.assertRaisesRegex(ValueError, "unique"):
            tasks.validate(self.data)

    def test_unrenderable_text_is_rejected(self):
        self.data["tasks"][0]["title"] = "A tab\nNew line"
        with self.assertRaisesRegex(ValueError, "printable"):
            tasks.validate(self.data)

    def test_quoted_text_is_escaped(self):
        self.data["tasks"][0]["steps"][0] = 'Read "one" note'
        self.assertIn(r'\"one\"', tasks.generate(self.data))


if __name__ == "__main__":
    unittest.main()
